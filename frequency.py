
import re
import sys

def path_as_list_of_words(path):
    with open(path, 'r') as f:
        content = f.read()
        words = re.split('[^a-zA-Z]', content)
    return words

def frequency(path_book, path_stop_words, n) : 
    dict = {}
    words = path_as_list_of_words(path_book)
    stop_words = path_as_list_of_words(path_stop_words)
    for w in words: 
        m = w.lower()
        if len(m)>1 and m not in stop_words:
            if m in dict:
                dict[m] += 1
            else:
                dict[m]= 1
    sorted_dict = sorted(dict.items(), key=lambda item:item[1], reverse=True)
    for i in range(n):
        print(sorted_dict[i])

if len(sys.argv) != 3:
    print("Il faut deux paramètres, le chemin du fichier txt et le nombre de mots")
else:
    frequency(sys.argv[1], "stop_words.txt", int(sys.argv[2]))