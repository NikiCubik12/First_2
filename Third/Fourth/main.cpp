#include <chrono>
#include <clocale>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include "MutableArraySequence.hpp"
#include "ReadOnlyStream.hpp"
#include "PatternCounter.hpp"
#include "LazySequence.hpp"
#include "Generator.hpp"

namespace
{

std::string ReadLine(const std::string& prompt)
{
    std::cout << prompt;
    std::cout.flush();
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int ReadInt(const std::string& prompt, int fallback)
{
    setlocale(LC_ALL, "Rus");
    std::string line = ReadLine(prompt);
    if (line.empty()) return fallback;
    try { return std::stoi(line); }
    catch (...) { return fallback; }
}

MutableArraySequence<std::string> ReadPatterns()
{
    std::cout << "Введите подстроки (по одной на строке, пустая строка — конец):\n";
    MutableArraySequence<std::string> out;
    while (true)
    {
        std::string s;
        if (!std::getline(std::cin, s)) break;
        if (s.empty()) break;
        out.Append(s);
    }
    return out;
}

std::shared_ptr<ReadOnlyStream<char>> MakeKeyboardStream()
{
    std::cout << "Введите строку для поиска:\n";
    std::string s;
    std::getline(std::cin, s);
    return std::make_shared<ReadOnlyStream<char>>(s);
}

std::shared_ptr<ReadOnlyStream<char>> MakeFileStream()
{
    std::string path = ReadLine("Путь к файлу: ");
    auto fin = std::make_shared<std::ifstream>(path, std::ios::binary);
    if (!fin->is_open())
    {
        std::cout << "Не удалось открыть файл: " << path << '\n';
        return nullptr;
    }
    Deserializer<char> deser = [](std::istream& s, char& v) -> bool {
        char c;
        if (s.get(c)) { v = c; return true; }
        return false;
    };
    return std::make_shared<ReadOnlyStream<char>>(fin, deser);
}

std::shared_ptr<LazySequence<char>> MakeGeneratedLazy()
{
    int len = ReadInt("Длина (по умолчанию 1000000): ", 1000000);
    std::string alphabet = ReadLine("Алфавит (по умолчанию 'ab'): ");
    if (alphabet.empty()) alphabet = "ab";

    auto rule = [alphabet](const BoundedQueue<char>&) -> char {
        static std::mt19937 rng(42);
        std::uniform_int_distribution<size_t> dist(0, alphabet.size() - 1);
        return alphabet[dist(rng)];
    };
    MutableArraySequence<char> init;
    init.Append(alphabet[0]);

    auto gen = std::make_shared<Generator<char>>(rule, init, 1, static_cast<size_t>(len));
    return std::make_shared<LazySequence<char>>(*gen);
}

void PrintCounts(Sequence<std::pair<std::string, std::size_t>>* counts)
{
    std::cout << "\n=== Результаты ===\n";
    for (std::size_t i = 0; i < counts->GetLength(); ++i)
    {
        auto p = counts->Get(i);
        std::cout << "  \"" << p.first << "\" : " << p.second << '\n';
    }
}

void RunCounter(PatternCounter& pc,
                ReadOnlyStream<char>* stream,
                LazySequence<char>*   lazy)
{
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();

    const size_t reportEvery = 100000;
    size_t lastReport = 0;
    auto progress = [&]() {
        size_t n = pc.GetProcessedCount();
        if (n - lastReport >= reportEvery)
        {
            lastReport = n;
            std::cout << "\r... обработано символов: " << n << std::flush;
        }
    };

    if (stream)
    {
        while (!stream->IsEndOfStream())
        {
            pc.ConsumeChar(stream->Read());
            progress();
        }
    }
    else if (lazy)
    {
        size_t i = 0;
        while (true)
        {
            try { pc.ConsumeChar(lazy->Get(i)); ++i; progress(); }
            catch (const IndexOutOfRangeException&) { break; }
        }
    }
    std::cout << "\r... обработано символов: " << pc.GetProcessedCount() << '\n';

    auto t1 = clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "Время: " << ms << " мс\n";
}

void PrintMenu()
{
    std::cout << "\n=== ЛР-4. Вариант 8: частоты подстрок ===\n"
              << "1. Источник — клавиатура\n"
              << "2. Источник — файл\n"
              << "3. Источник — сгенерированная LazySequence\n"
              << "4. Автоматический пример (классика 'ushers')\n"
              << "0. Выход\n";
}

void RunAutoExample()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("he"));
    pats.Append(std::string("she"));
    pats.Append(std::string("his"));
    pats.Append(std::string("hers"));
    PatternCounter pc(pats, true);

    ReadOnlyStream<char> rs(std::string("ushers"));
    while (!rs.IsEndOfStream()) pc.ConsumeChar(rs.Read());

    auto* counts = pc.GetCounts();
    std::cout << "Вход: \"ushers\"\nПодстроки: he, she, his, hers\nОверлап: ON\n";
    PrintCounts(counts);
    delete counts;
}

} // namespace

int main()
{
    setlocale(LC_ALL, "Rus");

    while (true)
    {
        PrintMenu();
        std::string choice = ReadLine("> ");
        if (choice == "0") break;

        if (choice == "4") { RunAutoExample(); continue; }

        std::shared_ptr<ReadOnlyStream<char>> stream;
        std::shared_ptr<LazySequence<char>>   lazy;

        if      (choice == "1") stream = MakeKeyboardStream();
        else if (choice == "2") stream = MakeFileStream();
        else if (choice == "3") lazy   = MakeGeneratedLazy();
        else { std::cout << "Неизвестный пункт\n"; continue; }

        if (!stream && !lazy) continue;

        auto patterns = ReadPatterns();
        if (patterns.GetLength() == 0)
        {
            std::cout << "Список подстрок пуст\n";
            continue;
        }
        std::string ov = ReadLine("Считать перекрытия? (y/n, по умолч. y): ");
        bool allowOverlap = !(ov == "n" || ov == "N");

        PatternCounter pc(patterns, allowOverlap);
        RunCounter(pc, stream.get(), lazy.get());

        auto* counts = pc.GetCounts();
        PrintCounts(counts);
        delete counts;
    }
    std::cout << "Пока!\n";
    return 0;
}
