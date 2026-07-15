import sqlite3
import os

db = sqlite3.connect('data/dictionary.db')
cur = db.cursor()

# Сколько слов
cur.execute('SELECT COUNT(*) FROM words')
print(f'Слов: {cur.fetchone()[0]}')

# Сколько переводов
cur.execute('SELECT COUNT(*) FROM translations')
print(f'Переводов: {cur.fetchone()[0]}')

# Пример — найти слово
cur.execute('''
    SELECT w.kanji, w.reading, t.language, t.meaning
    FROM words w
    JOIN translations t ON w.id = t.word_id
    WHERE w.kanji = '食べる'
''')

for row in cur.fetchall():
    print(row)

db.close()
