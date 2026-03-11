//
// Created by Ксения on 01.03.2026.
//

#include <iostream>
#include <algorithm>

class String {
private:
    char* Data;
    size_t length;

public:
    String() {
        length = 0;
        Data = new char[1];
        Data[0] = '\0';
    }
    // копирование
    String(const String& rhs) {
        length = rhs.length;                   // копия длину
        Data = new char[length + 1];           // память +1 для 0
        std::copy(rhs.Data, rhs.Data + length, Data);
        Data[length] = '\0';
    }
    // перемещение
    String(String&& rhs) {
        Data = rhs.Data;
        length = rhs.length;
        rhs.Data = new char[1];
        rhs.Data[0] = '\0';
        rhs.length = 0;
    }

    // с-строка
    String(const char* data) {
        length = 0;
        if (data) {
            while (data[length] != '\0') {
                length++;
            }
        }
        Data = new char[length + 1];
        if (data) {
            std::copy(data, data + length, Data);
        }
        Data[length] = '\0';
    }
    //деструктор
    ~String() {
        delete[] Data;
    }
    //оператор копирования
    String& operator=(const String& rhs) {
        if (&rhs != this) {
            delete[] Data;
            length = rhs.length;
            Data = new char[length + 1];
            std::copy(rhs.Data, rhs.Data + length, Data);
            Data[length] = '\0';
        }
        return *this;
    }

    String& operator=(String&& rhs) {
        if (&rhs != this) {
            delete[] Data;
            Data = rhs.Data;
            length = rhs.length;

            rhs.Data = new char[1];
            rhs.Data[0] = '\0';
            rhs.length = 0;
        }
        return *this;
    }

    String& operator+=(const String& rhs) {
        char* temp = new char[length + rhs.length + 1];
        std::copy(Data, Data + length, temp);
        std::copy(rhs.Data, rhs.Data + rhs.length, temp + length);
        length += rhs.length;
        temp[length] = '\0';
        delete[] Data;
        Data = temp;
        return *this;
    }
//умножение строки
    String& operator*=(unsigned int m) {
        if (m == 0) {
            delete[] Data;
            length = 0;
            Data = new char[1];
            Data[0] = '\0';
            return *this;
        }
        char* temp = new char[length * m + 1];
        for (unsigned int i = 0; i < m; i++) {
            std::copy(Data, Data + length, temp + i * length);
        }
        length *= m;
        temp[length] = '\0';
        delete[] Data;
        Data = temp;
        return *this;
    }
//оператор сравнения
    bool operator==(const String& rhs) const {
        if (length != rhs.length) return false;
        for (size_t i = 0; i < length; i++) {
            if (Data[i] != rhs.Data[i]) return false;
        }
        return true;
    }
    bool operator<(const String& rhs) const {
        for (size_t i = 0; i < std::min(length, rhs.length); i++) {
            if (Data[i] < rhs.Data[i]) return true;
            if (Data[i] > rhs.Data[i]) return false;
        }
        return length < rhs.length;
    }
//поиск подстроки
    long long Find(const String& substr) const {
        // если подстрока длиннее строки, её не может быть
        if (length < substr.length) return -1;
        for (size_t i = 0; i <= length - substr.length; i++) {
            bool found = true;
            for (size_t j = 0; j < substr.length; j++) {
                if (Data[i + j] != substr.Data[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return i;
        }
        return -1;
    }
//замена символов
    void Replace(char oldSymbol, char newSymbol) {
        for (size_t i = 0; i < length; i++) {
            if (Data[i] == oldSymbol) {
                Data[i] = newSymbol;
            }
        }
    }
    size_t Size() const {
        return length;
    }
    bool Empty() const {
        return length == 0;
    }
    char operator[](size_t index) const {
        return Data[index];
    }
    char& operator[](size_t index) {
        return Data[index];
    }
//обрезка справа символов
    void RTrim(char symbol) {
        size_t new_len = length;
        while (new_len > 0 && Data[new_len - 1] == symbol) {
            new_len--;
        }
        if (new_len == length) return;
        char* temp = new char[new_len + 1];
        std::copy(Data, Data + new_len, temp);
        temp[new_len] = '\0';
        delete[] Data;
        Data = temp;
        length = new_len;
    }
//обрезка слева
    void LTrim(char symbol) {
        size_t start = 0;
        while (start < length && Data[start] == symbol) {
            start++;
        }
        if (start == 0) return;
        if (start == length) {
            delete[] Data;
            length = 0;
            Data = new char[1];
            Data[0] = '\0';
            return;
        }
        size_t new_len = length - start;
        char* temp = new char[new_len + 1];
        std::copy(Data + start, Data + start + new_len, temp);
        temp[new_len] = '\0';
        delete[] Data;
        Data = temp;
        length = new_len;
    }
//обмен содержимого
    void swap(String& oth) {
        std::swap(Data, oth.Data);
        std::swap(length, oth.length);
    }

    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << str.Data;
        return os;
    }
};

String operator+(const String& a, const String& b) {
    String result(a);
    result += b;
    return result;
}

String operator*(const String& a, unsigned int b) {
    String result(a);
    result *= b;
    return result;
}

bool operator!=(const String& a, const String& b) {
    return !(a == b);
}

bool operator>(const String& a, const String& b) {
    return b < a;
}
//демонстрация
void demonstrateMoveSemantics() {
    String source("Hello World");
    std::cout << "   source: \"" << source << "\"\n";

    String dest1(std::move(source));
    std::cout << "   dest1: \"" << dest1 << "\"\n";
    std::cout << "   source: \"" << source << "\" (empty, valid state)\n";

    String source2("Move me");
    std::cout << "\n   source2: \"" << source2 << "\"\n";

    String dest2;
    dest2 = std::move(source2);
    std::cout << "   dest2: \"" << dest2 << "\"\n";
    std::cout << "   source2: \"" << source2 << "\" (empty, valid state)\n";
}
int main() {
    std::cout << "CONSTRUCTORS\n";
    String s1;
    std::cout << "s1 (empty): \"" << s1 << "\"\n";
    String s2("Hello");
    std::cout << "s2 (from C-string): \"" << s2 << "\"\n";
    String s3(s2);
    std::cout << "s3 (copy of s2): \"" << s3 << "\"\n\n";

    std::cout << "ASSIGNMENT OPERATOR\n";
    String s4;
    s4 = s2;
    std::cout << "s4 = s2: \"" << s4 << "\"\n\n";

    std::cout << "OPERATOR +=\n";
    String s5("World");
    s2 += " ";
    s2 += s5;
    std::cout << "s2 += \" \" += s5 = \"" << s2 << "\"\n\n";

    // 4. Оператор []
    std::cout << "OPERATOR []\n";
    std::cout << "s2[0] = " << s2[0] << "\n";
    s2[0] = 'J';
    std::cout << "After s2[0] = 'J': \"" << s2 << "\"\n\n";

    std::cout << "OPERATOR *=\n";
    String s6("Ha");
    s6 *= 3;
    std::cout << "s6 *= 3 = \"" << s6 << "\"\n\n";

    std::cout << "COMPARISON OPERATORS\n";
    String s7("Apple");
    String s8("Banana");
    String s9("Apple");
    std::cout << "s7 == s8: " << (s7 == s8 ? "true" : "false") << "\n";
    std::cout << "s7 == s9: " << (s7 == s9 ? "true" : "false") << "\n";
    std::cout << "s7 < s8: " << (s7 < s8 ? "true" : "false") << "\n\n";

    std::cout << "FIND METHOD\n";
    String s10("Hello world");
    std::cout << "In string \"" << s10 << "\"\n";
    std::cout << "'world' at position: " << s10.Find("world") << "\n\n";

    std::cout << "REPLACE METHOD\n";
    String s11("Hello world");
    std::cout << "Before: \"" << s11 << "\"\n";
    s11.Replace('o', '0');
    std::cout << "After: \"" << s11 << "\"\n\n";

    std::cout << "TRIM METHODS\n";
    std::cout << "----------------\n";

    String s12("___Hello___");
    std::cout << "Before: \"" << s12 << "\"\n";
    s12.LTrim('_');
    std::cout << "LTrim: \"" << s12 << "\"\n";
    s12.RTrim('_');
    std::cout << "RTrim: \"" << s12 << "\"\n\n";

    std::cout << "SWAP METHOD\n";
    String s13("First");
    String s14("Second");
    std::cout << "Before swap: " << s13 << " " << s14 << "\n";
    s13.swap(s14);
    std::cout << "After swap: " << s13 << " " << s14 << "\n\n";

    std::cout << "GLOBAL OPERATORS\n";
    String s15("Hello");
    String s16("World");
    std::cout << "s15 + s16 = \"" << s15 + s16 << "\"\n";
    std::cout << "s15 * 3 = \"" << s15 * 3 << "\"\n";
    return 0;
}