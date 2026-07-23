import json
import sqlite3
import glob
import zipfile
import requests
from tqdm import tqdm
from urllib.parse import urlparse
import os
from pathlib import Path

# Downloading packages in selected languages
def download(url, directory):
    try:
        filename = os.path.basename(urlparse(url).path)
        filepath = os.path.join(directory, filename)

        response = requests.get(url, stream=True)
        response.raise_for_status()

        total_size = int(response.headers.get('content-length', 0))

        with open(filepath, 'wb') as file, \
            tqdm(desc=filename,
                 total=total_size,
                 unit='iB',
                 unit_scale=True) as progress_bar:

            for chunk in response.iter_content(chunk_size=8192):
                size = file.write(chunk)
                progress_bar.update(size)

        return filepath
    except requests.exceptions.RequestException as e:
        print(f"Download error: {e}")
        return False;

# Gets text from jmdict files
def extract_text(item):
    if (isinstance(item, str)):
       return item
    elif isinstance(item, dict):
        if (item.get('type') == 'structured-content'):
            return extract_text(item.get('content', ''))
        elif ('content' in item):
            content = item['content']
            if isinstance(content, list):
                return ', '.join(extract_text(c) for c in content)
            return extract_text(content)
        elif (item.get('tag') == 'li'):
            return extract_text(item.get('content', ''))
    elif isinstance(item, list):
        return ', '.join(extract_text(i) for i in item)
    return ''

# Download cycle
def download_many(urls, directory):
    files = []
    for url in urls:
        downloaded_file = download(url, directory)
        if (downloaded_file):
            print(f"Downloaded file to {downloaded_file}")
            files.append(downloaded_file)
    return files

# Unzipping packages and deleting zip files
def unzip_and_delete_many(zips):
    extracted = []
    for file in zips:
        with zipfile.ZipFile(file, 'r') as currentZip:
            directory = file.removesuffix('.zip')
            currentZip.extractall(directory)
            extracted.append(directory)
            Path(file).unlink(missing_ok=True)
    return extracted

# Creating database files
def create_tables(cur):

    cur.execute('''
        CREATE TABLE words (
            id INTEGER PRIMARY KEY,
            kanji TEXT,
            reading TEXT,
            jlpt_level INTEGER DEFAULT 0
        );
        ''')

    cur.execute('''
        CREATE TABLE translations (
            id INTEGER PRIMARY KEY,
            word_id INTEGER,
            language TEXT,
            meaning TEXT,
            FOREIGN KEY (word_id) REFERENCES words(id)
        );
        ''')

    cur.execute('CREATE INDEX IF NOT EXISTS idx_kanji ON words(kanji);')
    cur.execute('CREATE INDEX IF NOT EXISTS idx_reading ON words(reading);')

# Filling up the dictionary
# External cycle for all downloaded languages
def fill_database_jmdict(cur, extract_dir):
    for language_dir in extract_dir:
        print (language_dir)
        language = os.path.basename(language_dir).removeprefix('JMdict_')

        # Internal cycle for files for language
        for file in glob.glob(os.path.join(language_dir, 'term_bank_*.json')):
            print(f'Reading {file}...')
            with open(file, encoding='UTF-8') as f:

                entries = json.load(f)
                
                for entry in entries:
                    kanji = entry[0]
                    reading = entry[1]
                    translations = entry[5]
                    translation = ', '.join(extract_text(t) for t in translations) \
                        if isinstance(translations, list) else str(translations)

                    cur.execute('SELECT id FROM words WHERE kanji = ?', (kanji,))

                    row = cur.fetchone()
                    if(row):
                        word_id = row[0]
                        cur.execute(
                            'INSERT INTO translations (language, meaning, word_id) VALUES (?, ?, ?)',
                            (language, translation, word_id)
                        )
                    else:
                     
                        cur.execute(
                            'INSERT INTO words (kanji, reading) VALUES (?, ?)',
                            (kanji, reading)
                        )

                        cur.execute('SELECT last_insert_rowid()')
                        word_id = cur.fetchone()[0]

                        cur.execute(
                            'INSERT INTO translations (language, meaning, word_id) VALUES (?, ?, ?)',
                            (language, translation, word_id)
                        )

# Searches for lines with "sS'q'" in file and yields each next line
def get_kanji_char(outfile):
    print_next = False
    for line in outfile:
        # Convert from \uXXXX format in kanji and yield
        if print_next:
            try:
                trimmedLine = line.rstrip('\n')[2:-1]
                convertedLine = trimmedLine.encode('utf-8').decode('unicode_escape')
            except:
                print(f"Error while decoding {os.path.basename(outfile)}: {line}")
                convertedLine = "Error"
            print_next = False
            yield convertedLine
        if "sS'q'" in line:
            print_next = True

# Find entry in database and add a jlpt level
def database_write_levels(cur, level, kanji):
    cur.execute('SELECT COUNT(*) FROM words WHERE kanji = ?', (kanji,))
    if(cur.fetchone()[0] == 0):
        print("Not found")
    cur.execute('UPDATE words SET jlpt_level = ? WHERE kanji = ?', (level, kanji)) 

# Cycles through kanji and vocabulary files that contain jlpt levels
def add_jlpt_levels(cur, kanji, vocab):
    # Adding kanji levels to database
    for file in kanji:
        level = int(os.path.basename(file)[1:2])
        # Write only needed lines from outfile
        with open (file, 'r', encoding='cp932') as outfile:
            for characters in get_kanji_char(outfile):
                database_write_levels(cur, level, characters)

    for file in vocab:
        level = int(os.path.basename(file)[1:2])
        with open (file, 'r', encoding='cp932') as outfile:
            for characters in get_kanji_char(outfile):
                database_write_levels(cur, level, characters)

# Adding vocabulary levels to database
def fillDatabase(dbConn, directories):
    cursor = dbConn.cursor()
    fill_database_jmdict(cursor, directories.get("jmdict"))
    dbConn.commit()

    add_jlpt_levels(cursor, directories.get("kanji"), directories.get("vocab"))
    dbConn.commit()

    dbConn.close()

