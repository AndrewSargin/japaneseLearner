import os
import helpingFuncs as helpers

from pathlib import Path

# All paths used in script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(os.path.dirname(SCRIPT_DIR))
db_DIR = os.path.join(PROJECT_DIR, "data/dictionary.db")
jmDict_DIR = Path(os.path.join(PROJECT_DIR, "data/yomitan"))
jlptLevels_DIR = Path(os.path.join(PROJECT_DIR, "data/tanos_co_uk"))
jlptVocab_DIR = Path(os.path.join(jlptLevels_DIR, "vocabulary"))
jlptKanji_DIR = Path(os.path.join(jlptLevels_DIR, "kanji"))

# Create directories if they don't exist
jmDict_DIR.mkdir(parents=True, exist_ok=True)
jlptLevels_DIR.mkdir(parents=True, exist_ok=True)
jlptVocab_DIR.mkdir(parents=True, exist_ok=True)
jlptKanji_DIR.mkdir(parents=True, exist_ok=True)

# Urls for JMdict
urls_yomitan = [
    "https://github.com/yomidevs/jmdict-yomitan/releases/latest/download/JMdict_russian.zip",
    "https://github.com/yomidevs/jmdict-yomitan/releases/latest/download/JMdict_english.zip"
    ]

# Urls for vocabulary of jlpt levels
urls_jlpt_vocab = [
    "https://www.tanos.co.uk/jlpt/jlpt1/vocab/n1-vocab-kanji-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt2/vocab/n2-vocab-kanji-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt3/vocab/n3-vocab-kanji-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt4/vocab/n4-vocab-kanji-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt5/vocab/n5-vocab-kanji-eng.mem"
    ]

# Urls for kanji of jlpt levels
urls_jlpt_kanji = [
    "https://www.tanos.co.uk/jlpt/jlpt1/kanji/n1-kanji-char-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt2/kanji/n2-kanji-char-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt3/kanji/n3-kanji-char-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt4/kanji/n4-kanji-char-eng.mem",
    "https://www.tanos.co.uk/jlpt/jlpt5/kanji/n5-kanji-char-eng.mem"
    ]

# Download all files
jmDictFiles = helpers.download_many(urls_yomitan, jmDict_DIR)
kanjiLevelsFiles = helpers.download_many(urls_jlpt_kanji, jlptKanji_DIR)
vocabLevelFiles = helpers.download_many(urls_jlpt_vocab, jlptVocab_DIR)

# Unzip JMdict archives
jmDictDirs = helpers.unzip_and_delete_many(jmDictFiles)

# Delete database before creating
Path(db_DIR).unlink(missing_ok=True)

directories = {"jmdict": jmDictDirs, "kanji": kanjiLevelsFiles, "vocab": vocabLevelFiles}

dbConn = helpers.create_tables(db_DIR)
helpers.fillDatabase(dbConn, directories)
