import json
import glob
import zipfile
import requests
from tqdm import tqdm
from urllib.parse import urlparse
import os
from pathlib import Path
import databaseHelpers as dataHelp

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

# Creating all tables and indexes
def create_tables(cur):

    cur.execute('''
        CREATE TABLE entry (
            id INTEGER PRIMARY KEY,
            jlpt_level INTEGER DEFAULT 0
        );
    ''')

    cur.execute('''
        CREATE TABLE kanji (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            entry_id INTEGER NOT NULL,
            text TEXT not NULL,
            common BOOLEAN NOT NULL,

            FOREIGN KEY (entry_id) REFERENCES entry(id)
        );
    ''')

    cur.execute('''
        CREATE TABLE kana (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            entry_id INTEGER NOT NULL,
            text TEXT not NULL,
            common BOOLEAN NOT NULL,

            FOREIGN KEY (entry_id) REFERENCES entry(id)
        );
    ''')

    cur.execute('''
        CREATE TABLE meaning (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            entry_id INTEGER NOT NULL,
            order_no INTEGER NOT NULL,

            FOREIGN KEY (entry_id) REFERENCES entry(id)
        );
    ''')

    cur.execute(''' 
        CREATE TABLE translation (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            meaning_id INTEGER NOT NULL,
            language TEXT NOT NULL,
            text TEXT NOT NULL,

            FOREIGN KEY (meaning_id) REFERENCES meaning(id)
        );
    ''')

    cur.execute('''
        CREATE TABLE tag (
            id INTEGER PRIMARY KEY,
            tag TEXT UNIQUE NOT NULL
        );
    ''')

    cur.execute('''
        CREATE TABLE meaning_pos (
            meaning_id INTEGER NOT NULL,
            pos_id INTEGER NOT NULL,

            PRIMARY KEY (meaning_id, pos_id),

            FOREIGN KEY (meaning_id) REFERENCES meaning(id),
            FOREIGN KEY (pos_id) REFERENCES pos(id)
        );
    ''')

def create_indices(cur):
    cur.execute('CREATE INDEX idx_kanji_text ON kanji(text);')
    cur.execute('CREATE INDEX idx_kana_text ON kana(text);')
    cur.execute('CREATE INDEX idx_translation_lang ON translation(language);')
    cur.execute('CREATE INDEX idx_translation_meaning ON translation(meaning_id);')
    cur.execute('CREATE INDEX idx_kanji_entry ON kanji(entry_id);')
    cur.execute('CREATE INDEX idx_kana_entry ON kana(entry_id);')
    cur.execute('CREATE INDEX idx_meaning_entry ON meaning(entry_id);')
    cur.execute('CREATE INDEX idx_translation_meaning_lang ON translation(meaning_id, language);')
    cur.execute('ANALYZE;')
"""
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
"""

def fill_database_jmdict(cur, directories):
    cache = dataHelp.CacheImporter()
    cache.load_db_cache(cur)
    for langDir in directories:
        for file in glob.glob(os.path.join(langDir, '*.json')):
            with open(file, 'r', encoding='UTF-8') as f:

                data = json.load(f)
                count = len(data['words'])
                for idx, word in enumerate(data['words']):
                    if idx%1000 == 0:
                        print(f'{idx} / {count}')

                    dataHelp.insert_entry(cur, word['id'], cache)
                    dataHelp.insert_kanji(cur, word['id'], word['kanji'], cache)
                    dataHelp.insert_kana(cur, word['id'], word['kana'], cache)
                    dataHelp.insert_meaning(cur, word['id'], word['sense'], cache)

"""
# Filling up the dictionary
# External cycle for all downloaded languages
/*def fill_database_jmdict(cur, extract_dir):
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
                    # If a word already exists in database, add only translation
                    if(row):
                        word_id = row[0]
                        cur.execute(
                            'INSERT INTO translations (language, meaning, word_id) VALUES (?, ?, ?)',
                            (language, translation, word_id)
                        )
                    # Add a word an it's translation to database
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
"""

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
    cur.execute('''UPDATE entry SET jlpt_level = ? 
        WHERE id IN (
            SELECT entry_id FROM kanji WHERE text = ?)''',
        (level, kanji))
    if cur.rowcount == 0:
        print('Not found')
    else: print ('ok')

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
def setup_database(cursor):

    cursor.execute('PRAGMA journal_mode=OFF')
    cursor.execute('PRAGMA synchronous=OFF')
    cursor.execute('PRAGMA temp_store=MEMORY')
    cursor.execute('PRAGMA cache_size=-100000')

def get_jmdict_urls(languages):
    githubApiUrl = "https://api.github.com/repos/scriptin/jmdict-simplified/releases/latest"

    release = requests.get(githubApiUrl).json()
    tagname = release["tag_name"]
    urls = []

    for asset in release["assets"]:
        for lang in languages:
            if asset["name"].startswith(f"jmdict-{lang}-{tagname}") and asset["name"].endswith(".json.zip"):
                urls.append(asset["browser_download_url"])

    return urls

    





