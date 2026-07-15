#!/usr/bin/env python3

import json
from os.path import join
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

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(os.path.dirname(SCRIPT_DIR))
directory = os.path.join(PROJECT_DIR, "data/yomitan")

urls = [
    "https://github.com/yomidevs/jmdict-yomitan/releases/latest/download/JMdict_russian.zip",
    "https://github.com/yomidevs/jmdict-yomitan/releases/latest/download/JMdict_english.zip"
    ]

zips = []
extract_dir = []

# Download cycle
for url in urls:
    downloaded_file = download(url, directory)
    if (downloaded_file):
        print(f"Downloaded file to {downloaded_file}")
        zips.append(downloaded_file)

# Unzipping packages and deleting zip files
for file in zips:
    with zipfile.ZipFile(file, 'r') as currentZip:
        directory = file.removesuffix('.zip')
        currentZip.extractall(directory)
        extract_dir.append(directory)
        Path(file).unlink(missing_ok=True)

db_path = os.path.join(PROJECT_DIR, "data/dictionary.db")
Path(db_path).unlink(missing_ok=True)

# Creating database files
db = sqlite3.connect(db_path)

cur = db.cursor()

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
                print(translations)
                translation = ', '.join(extract_text(t) for t in translations) if isinstance(translations, list) else str(translations)

                cur.execute('SELECT id FROM words WHERE kanji = ?', (kanji,))

                row = cur.fetchone()
                if(row):
                    word_id = row[0]
                    cur.execute(
                        'INSERT INTO translations (language, meaning, word_id) VALUES (?, ?, ?)',
                        (language, translation, word_id)
                    )
                else:
                        

                    # Have to make ids from 'words' go to 'translations'
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

db.commit()
db.close()
print('Database is ready.')
