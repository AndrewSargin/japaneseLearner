import sqlite3

# 1. Подключаемся к вашей базе данных
conn = sqlite3.connect('data/dictionary.db')
cursor = conn.cursor()

try:
    # 2. Достаем первые 5 строк из таблицы (замените названия столбца и таблицы на ваши)
    # Предполагаем, что столбец называется character, а таблица kanji_table
    cursor.execute("SELECT kanji FROM words LIMIT 5")
    rows = cursor.fetchall()
    
    print("--- АНАЛИЗ СТРОК ИЗ БАЗЫ ДАННЫХ ---")
    if not rows:
        print("Таблица пустая или столбец указан неверно!")
    
    for index, row in enumerate(rows, 1):
        db_kanji = row[0]
        
        # Если в ячейке вдруг None, пропустим её
        if db_kanji is None:
            print(f"Строка {index}: В базе хранится NULL (пустота)")
            continue
            
        # Печатаем визуальный вид и технический (repr)
        print(f"Строка {index}:")
        print(f"  Как выглядит на экране: '{db_kanji}'")
        print(f"  Как видит Python (repr): {repr(db_kanji)}")
        print(f"  Длина значения в БД : {len(str(db_kanji))} симв.")
        print("-" * 35)

except sqlite3.Error as e:
    print(f"Ошибка при чтении базы данных: {e}")
finally:
    conn.close()

