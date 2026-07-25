class CacheImporter:
    def __init__(self):
            self.entries = set()
            self.kanji = set()
            self.kana = set()
            self.meanings = {}
            self.translations = set()

    def load_db_cache(self, cur):

        # Entry
        cur.execute('SELECT id FROM entry')
        self.entries = {row[0] for row in cur.fetchall()}

        # Kanji
        cur.execute('SELECT entry_id, text FROM kanji')
        self.kanji = {(row[0], row[1]) for row in cur.fetchall()}

        # Kana
        cur.execute('SELECT entry_id, text FROM kana')
        self.kana = {(row[0], row[1]) for row in cur.fetchall()}

        # Meaning
        cur.execute('SELECT id, entry_id, order_no FROM meaning')
        self.meanings = {
            (row[1], row[2]) : row[0]
            for row in cur.fetchall()
        }

        # Translation
        cur.execute('SELECT meaning_id, language, text FROM translation')
        self.translations = {
            (row[0], row[1], row[2])
            for row in cur.fetchall()
        }


def insert_entry(cur, wordId, cache):
    if (wordId not in cache.entries):
        cur.execute('INSERT INTO entry (id) VALUES (?)', (wordId,))
        cache.entries.add(wordId)

def insert_kanji(cur, wordId, kanjis, cache):
    for kanji in kanjis:
        key = (wordId, kanji['text'])

        if(key not in cache.kanji):
            cur.execute('INSERT INTO kanji (entry_id, text, common) VALUES (?, ?, ?)', \
                (wordId, kanji['text'], kanji['common']))
            cache.kanji.add(key)

def insert_kana(cur, wordId, kanas, cache):
    for kana in kanas:
        key = (wordId, kana['text'])

        if(key not in cache.kana):
            cur.execute('INSERT INTO kana (entry_id, text, common) VALUES (?, ?, ?)', \
                (wordId, kana['text'], kana['common']))
            cache.kana.add(key)

def insert_translation(cur, meaningId, translations, cache):
    for translation in translations:
        key = (meaningId, translation['lang'], translation['text'])
        
        if(key not in cache.translations):
            cur.execute('INSERT INTO translation (meaning_id, language, text) VALUES (?, ?, ?)', \
                (meaningId, translation['lang'], translation['text']))
            cache.translations.add(key)



def insert_meaning(cur, wordId, meanings, cache):
    for idx, meaning in enumerate(meanings):
        key = (wordId, idx)
        
        if(key in cache.meanings):
            meaningId = cache.meanings[key]
        else:
            cur.execute('INSERT INTO meaning (entry_id, order_no) VALUES (?, ?)', \
                (wordId, idx))
            meaningId = cur.lastrowid
            cache.meanings[key] = meaningId
        
        insert_translation(cur, meaningId, meaning['gloss'], cache)        
