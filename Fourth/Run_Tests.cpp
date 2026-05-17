#include <iostream>
#include <stdexcept>
#include <clocale>
#include "MutableArraySequence.hpp"
#include "Option.hpp"
#include "Cardinal.hpp"
#include "BoundedQueue.hpp"
#include "Generator.hpp"
#include "LazySequence.hpp"
#include "ReadOnlyStream.hpp"
#include "WriteOnlyStream.hpp"
#include "PatternCounter.hpp"
#include "KmpCounter.hpp"
#include <sstream>
#include <memory>
#include <string>
#include <climits>

static int g_failed = 0;
static int g_total  = 0;

#define CHECK(cond, name)                                            \
    do {                                                             \
        ++g_total;                                                   \
        if (!(cond)) {                                               \
            ++g_failed;                                              \
            std::cerr << "[FAIL] " << name << " (" << #cond << ")\n";\
        } else {                                                     \
            std::cout << "[ OK ] " << name << '\n';                  \
        }                                                            \
    } while (0)

static void smoke_sequence()
{
    setlocale(LC_All, "Rus");
    MutableArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);

    CHECK(seq.GetLength() == 3,  "smoke: длина 3");
    CHECK(seq.GetFirst()  == 10, "smoke: первый элемент");
    CHECK(seq.GetLast()   == 30, "smoke: последний элемент");
    CHECK(seq.Get(1)      == 20, "smoke: средний элемент");
}

static void test_optional()
{
    Optional<int> none = Optional<int>::None();
    Optional<int> some = Optional<int>::Some(42);

    CHECK(!none.HasValue(),            "Optional: None.HasValue == false");
    CHECK(some.HasValue(),             "Optional: Some.HasValue == true");
    CHECK(some.GetValue() == 42,       "Optional: Some.GetValue == 42");
    CHECK(none.OrElse(7) == 7,         "Optional: None.OrElse возвращает fallback");
    CHECK(some.OrElse(7) == 42,        "Optional: Some.OrElse возвращает значение");

    bool threw = false;
    try { (void)none.GetValue(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw, "Optional: None.GetValue бросает исключение");
}

static void test_cardinal()
{
    Cardinal zero;
    Cardinal three(3);
    Cardinal inf = Cardinal::Infinity();

    CHECK(zero.IsFinite(),            "Cardinal: по умолчанию финитен");
    CHECK(zero.GetFinite() == 0,      "Cardinal: по умолчанию = 0");
    CHECK(three.GetFinite() == 3,     "Cardinal: значение 3");
    CHECK(inf.IsInfinite(),           "Cardinal: Infinity бесконечен");
    CHECK(three < inf,                "Cardinal: 3 < Infinity");
    CHECK(!(inf < three),             "Cardinal: !(Infinity < 3)");
    CHECK(inf == Cardinal::Infinity(),"Cardinal: Infinity == Infinity");
    CHECK(Cardinal(5) > Cardinal(3),  "Cardinal: 5 > 3");

    bool threw = false;
    try { (void)inf.GetFinite(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw, "Cardinal: GetFinite на Infinity бросает исключение");
}

static void test_bounded_queue()
{
    BoundedQueue<int> q(3);
    q.Push(1); q.Push(2); q.Push(3);
    CHECK(q.IsFull() && q.Front() == 1 && q.Back() == 3,
          "BoundedQueue: заполнили 3 элементами");
    q.Push(4);
    CHECK(q.Front() == 2 && q.Back() == 4,
          "BoundedQueue: переполнение сдвинуло окно");

    BoundedQueue<int> copy = q;
    CHECK(copy.At(0) == 2 && copy.At(2) == 4, "BoundedQueue: копия корректна");

    bool threw = false;
    try { (void)q.At(10); } catch (const std::out_of_range&) { threw = true; }
    CHECK(threw, "BoundedQueue: At вне диапазона бросает исключение");
}

static MutableArraySequence<int> initialList(std::initializer_list<int> xs)
{
    MutableArraySequence<int> s;
    for (int x : xs) s.Append(x);
    return s;
}

static void test_generator_naturals()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1);

    CHECK(gen.HasNext(),            "Generator(нат): HasNext всегда true (бесконечный)");
    CHECK(gen.GetNext() == 1,       "Generator(нат): 1");
    CHECK(gen.GetNext() == 2,       "Generator(нат): 2");
    CHECK(gen.GetNext() == 3,       "Generator(нат): 3");
    CHECK(gen.GetNext() == 4,       "Generator(нат): 4");
    CHECK(gen.GetEmittedCount() == 4, "Generator(нат): эмитили 4 элемента");

    gen.Reset();
    CHECK(gen.GetNext() == 1, "Generator(нат): Reset возвращает к началу");
}

static void test_generator_fibonacci()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.At(0) + q.At(1); };
    auto init = initialList({1, 1});
    Generator<int> gen(rule, init, 2);

    int expected[] = {1, 1, 2, 3, 5, 8, 13, 21};
    bool ok = true;
    for (int e : expected)
        if (gen.GetNext() != e) ok = false;
    CHECK(ok, "Generator(Фибоначчи): 1,1,2,3,5,8,13,21");
}

static void test_generator_finite_bound()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() * 2; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 4);

    CHECK(gen.GetNext() == 1, "Generator(финит): 1");
    CHECK(gen.GetNext() == 2, "Generator(финит): 2");
    CHECK(gen.GetNext() == 4, "Generator(финит): 4");
    CHECK(gen.GetNext() == 8, "Generator(финит): 8");
    CHECK(!gen.HasNext(),     "Generator(финит): HasNext == false после 4");

    Optional<int> none = gen.TryGetNext();
    CHECK(!none.HasValue(),   "Generator(финит): TryGetNext = None");

    bool threw = false;
    try { (void)gen.GetNext(); } catch (const std::out_of_range&) { threw = true; }
    CHECK(threw, "Generator(финит): GetNext после конца бросает исключение");
}

static void test_generator_insert_at()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 5);
    gen.InsertAt(2, 99);

    int expected[] = {1, 2, 99, 3, 4, 5};
    bool ok = true;
    for (int e : expected)
        if (gen.GetNext() != e) ok = false;
    CHECK(ok, "Generator: InsertAt(2,99) → 1,2,99,3,4,5");
    CHECK(!gen.HasNext(), "Generator: после InsertAt границу расширили правильно");
}

static void test_generator_remove_at()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 5);
    gen.RemoveAt(1);

    int expected[] = {1, 3, 4, 5};
    bool ok = true;
    for (int e : expected)
        if (gen.GetNext() != e) ok = false;
    CHECK(ok, "Generator: RemoveAt(1) → 1,3,4,5");
    CHECK(!gen.HasNext(), "Generator: после RemoveAt границу сжали правильно");
}

static void test_generator_insert_subseq()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 4);
    auto ins = initialList({77, 88});
    gen.InsertSubsequence(2, ins);

    int expected[] = {1, 2, 77, 88, 3, 4};
    bool ok = true;
    for (int e : expected)
        if (gen.GetNext() != e) ok = false;
    CHECK(ok, "Generator: InsertSubsequence(2,[77,88]) → 1,2,77,88,3,4");
}

static void test_generator_try_get_next()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 2);
    Optional<int> a = gen.TryGetNext();
    Optional<int> b = gen.TryGetNext();
    Optional<int> c = gen.TryGetNext();
    CHECK(a.HasValue() && a.GetValue() == 1, "Generator::TryGetNext: первый = Some(1)");
    CHECK(b.HasValue() && b.GetValue() == 2, "Generator::TryGetNext: второй = Some(2)");
    CHECK(!c.HasValue(),                     "Generator::TryGetNext: третий = None");
}

static void test_lazy_from_sequence()
{
    MutableArraySequence<int> src;
    src.Append(10); src.Append(20); src.Append(30);
    LazySequence<int> lz(src);

    CHECK(lz.GetMaterializedCount() == 3, "LazySequence(seq): 3 элемента сразу в кэше");
    CHECK(!lz.IsInfinite(),               "LazySequence(seq): не бесконечная");
    CHECK(lz.GetFirst() == 10,            "LazySequence(seq): первый");
    CHECK(lz.GetLast()  == 30,            "LazySequence(seq): последний");
    CHECK(lz.Get(1)     == 20,            "LazySequence(seq): средний");
    CHECK(lz.GetLength() == 3,            "LazySequence(seq): длина 3");
    CHECK(lz.GetCardinality() == Cardinal::Finite(3), "LazySequence(seq): мощность = 3");
}

static void test_lazy_from_generator_finite()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 5);
    LazySequence<int> lz(gen);

    CHECK(!lz.IsInfinite(),                  "LazySequence(gen-fin): не бесконечная");
    CHECK(lz.GetMaterializedCount() == 0,    "LazySequence(gen-fin): кэш изначально пуст");
    CHECK(lz.Get(0) == 1 && lz.Get(2) == 3,  "LazySequence(gen-fin): элементы по индексу");
    CHECK(lz.GetMaterializedCount() == 3,    "LazySequence(gen-fin): кэш = 3 после Get(2)");
    CHECK(lz.GetLength() == 5,               "LazySequence(gen-fin): длина 5");
    CHECK(lz.GetMaterializedCount() == 5,    "LazySequence(gen-fin): кэш = 5 после GetLength");
}

static void test_lazy_from_generator_infinite()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1);
    LazySequence<int> lz(gen);

    CHECK(lz.IsInfinite(),               "LazySequence(gen-inf): бесконечная");
    CHECK(lz.GetCardinality() == Cardinal::Infinity(),
                                         "LazySequence(gen-inf): мощность = Infinity");
    CHECK(lz.Get(0) == 1,                "LazySequence(gen-inf): Get(0) = 1");
    CHECK(lz.Get(99) == 100,             "LazySequence(gen-inf): Get(99) = 100");
    CHECK(lz.GetMaterializedCount() == 100, "LazySequence(gen-inf): кэш = 100 после Get(99)");

    bool threw = false;
    try { (void)lz.GetLength(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw, "LazySequence(gen-inf): GetLength бросает исключение");
}

static void test_lazy_map_reduce()
{
    MutableArraySequence<int> src;
    src.Append(1); src.Append(2); src.Append(3); src.Append(4);
    LazySequence<int> lz(src);

    auto mapped = lz.template Map<int>([](int x) { return x * x; });
    CHECK(mapped->Get(0) == 1 && mapped->Get(3) == 16, "LazySequence::Map: квадраты");
    CHECK(mapped->GetLength() == 4,                    "LazySequence::Map: длина сохранилась");
    delete mapped;

    int sum = lz.Reduce([](int a, int b) { return a + b; }, 0);
    CHECK(sum == 10, "LazySequence::Reduce: сумма 1+2+3+4 = 10");
}

static void test_lazy_where()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1, 10);
    LazySequence<int> lz(gen);

    auto evens = lz.Where([](int x) { return x % 2 == 0; });
    CHECK(evens->Get(0) == 2 && evens->Get(4) == 10,
                                         "LazySequence::Where: чётные 2,4,6,8,10");
    CHECK(evens->GetLength() == 5,       "LazySequence::Where: длина 5");
    delete evens;
}

static void test_lazy_zip()
{
    auto ruleN = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto ruleS = [](const BoundedQueue<int>& q) { return q.Back() + q.Back(); };
    auto initN = initialList({1});
    auto initS = initialList({1});
    Generator<int> genN(ruleN, initN, 1);
    Generator<int> genS(ruleS, initS, 1);

    LazySequence<int> a(genN);
    LazySequence<int> b(genS);

    auto z = a.template Zip<int>(b);
    CHECK(z->Get(0).first == 1 && z->Get(0).second == 1,
                                        "LazySequence::Zip: (1,1)");
    CHECK(z->Get(3).first == 4 && z->Get(3).second == 8,
                                        "LazySequence::Zip: (4,8) на индексе 3");
    CHECK(z->IsInfinite(),              "LazySequence::Zip: inf+inf = inf");
    delete z;
}

static void test_lazy_append_concat()
{
    MutableArraySequence<int> src;
    src.Append(1); src.Append(2);
    LazySequence<int> lz(src);
    lz.Append(3);
    CHECK(lz.Get(2) == 3 && lz.GetLength() == 3, "LazySequence::Append: добавили 3");

    MutableArraySequence<int> tail;
    tail.Append(4); tail.Append(5);
    Sequence<int>* result = lz.Concat(&tail);
    CHECK(result->GetLength() == 5 && result->Get(4) == 5,
                                        "LazySequence::Concat: длина 5, последний 5");
    delete result;
}

static void test_ro_stream_from_sequence()
{
    MutableArraySequence<int> src;
    src.Append(7); src.Append(8); src.Append(9);
    ReadOnlyStream<int> rs(&src);

    CHECK(!rs.IsEndOfStream(), "ROStream(seq): не EOS");
    CHECK(rs.Read() == 7,      "ROStream(seq): Read 7");
    CHECK(rs.Read() == 8,      "ROStream(seq): Read 8");
    CHECK(rs.Read() == 9,      "ROStream(seq): Read 9");
    CHECK(rs.IsEndOfStream(),  "ROStream(seq): EOS после 3 чтений");

    bool threw = false;
    try { (void)rs.Read(); } catch (const EndOfStreamException&) { threw = true; }
    CHECK(threw, "ROStream(seq): Read после EOS бросает EndOfStreamException");
}

static void test_ro_stream_from_string()
{
    ReadOnlyStream<char> rs(std::string("abc"));
    CHECK(rs.Read() == 'a' && rs.Read() == 'b' && rs.Read() == 'c',
                                "ROStream(string): abc");
    CHECK(rs.IsEndOfStream(),   "ROStream(string): EOS");
}

static void test_ro_stream_from_lazy_infinite()
{
    auto rule = [](const BoundedQueue<int>& q) { return q.Back() + 1; };
    auto init = initialList({1});
    Generator<int> gen(rule, init, 1);
    LazySequence<int> lz(gen);
    ReadOnlyStream<int> rs(&lz);

    int sum = 0;
    for (int i = 0; i < 100; ++i) sum += rs.Read();
    CHECK(sum == 5050,             "ROStream(LazyInf): сумма 1..100 = 5050");
    CHECK(rs.GetPosition() == 100, "ROStream(LazyInf): position 100");
    CHECK(!rs.IsEndOfStream(),     "ROStream(LazyInf): бесконечный, не EOS");
}

static void test_ro_stream_from_istream()
{
    auto in = std::make_shared<std::istringstream>("11 22 33");
    Deserializer<int> deser = [](std::istream& s, int& v) -> bool {
        return static_cast<bool>(s >> v);
    };
    ReadOnlyStream<int> rs(in, deser);
    CHECK(rs.Read() == 11 && rs.Read() == 22 && rs.Read() == 33,
                                "ROStream(istream): 11,22,33");
    CHECK(rs.IsEndOfStream(),   "ROStream(istream): EOS после трёх");
}

static void test_ro_stream_seek_and_eos()
{
    MutableArraySequence<int> src;
    for (int i = 0; i < 5; ++i) src.Append(i * 10);
    ReadOnlyStream<int> rs(&src);

    CHECK(rs.IsCanSeek() && rs.IsCanGoBack(), "ROStream: умеет seek и goBack");
    rs.Seek(3);
    CHECK(rs.GetPosition() == 3 && rs.Read() == 30, "ROStream: Seek(3); Read = 30");
    rs.Seek(0);
    CHECK(rs.Read() == 0, "ROStream: Seek(0); Read = 0");

    rs.Close();
    bool threw = false;
    try { (void)rs.Read(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw, "ROStream: Read после Close бросает исключение");
}

static void test_wo_stream_to_sequence()
{
    MutableArraySequence<int> dst;
    WriteOnlyStream<int> ws(&dst);
    ws.Write(100); ws.Write(200); ws.Write(300);
    CHECK(ws.GetPosition() == 3, "WOStream(seq): position = 3");
    CHECK(dst.GetLength() == 3 && dst.Get(0) == 100 && dst.Get(2) == 300,
                                 "WOStream(seq): seq содержит 100,200,300");
}

static void test_wo_stream_to_ostream()
{
    auto out = std::make_shared<std::ostringstream>();
    Serializer<int> ser = [](std::ostream& s, const int& v) { s << v << ' '; };
    WriteOnlyStream<int> ws(out, ser);
    ws.Write(1); ws.Write(2); ws.Write(3);
    CHECK(out->str() == "1 2 3 ", "WOStream(ostream): \"1 2 3 \"");
}

static size_t pcCount(Sequence<std::pair<std::string, std::size_t>>* res, const std::string& key)
{
    for (size_t i = 0; i < res->GetLength(); ++i)
        if (res->Get(i).first == key) return res->Get(i).second;
    return SIZE_MAX;
}

static void consumeString(PatternCounter& pc, const std::string& s)
{
    for (char c : s) pc.ConsumeChar(c);
}

static void test_pc_classic_ushers()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("he"));
    pats.Append(std::string("she"));
    pats.Append(std::string("his"));
    pats.Append(std::string("hers"));

    PatternCounter pc(pats, true);
    consumeString(pc, "ushers");

    auto* res = pc.GetCounts();
    CHECK(pcCount(res, "he")   == 1, "PC(ushers): he = 1");
    CHECK(pcCount(res, "she")  == 1, "PC(ushers): she = 1");
    CHECK(pcCount(res, "his")  == 0, "PC(ushers): his = 0");
    CHECK(pcCount(res, "hers") == 1, "PC(ushers): hers = 1");
    delete res;
}

static void test_pc_empty_input()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("abc"));
    PatternCounter pc(pats, true);
    auto* res = pc.GetCounts();
    CHECK(pcCount(res, "abc") == 0, "PC(пустой вход): abc = 0");
    delete res;
}

static void test_pc_overlap_on()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("aa"));
    PatternCounter pc(pats, true);
    consumeString(pc, "aaaa");
    auto* res = pc.GetCounts();
    CHECK(pcCount(res, "aa") == 3, "PC(aa в aaaa, overlap=ON): 3");
    delete res;
}

static void test_pc_overlap_off()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("aa"));
    PatternCounter pc(pats, false);
    consumeString(pc, "aaaa");
    auto* res = pc.GetCounts();
    CHECK(pcCount(res, "aa") == 2, "PC(aa в aaaa, overlap=OFF): 2");
    delete res;
}

static void test_pc_from_stream()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("ab"));
    pats.Append(std::string("ba"));
    PatternCounter pc(pats, true);

    ReadOnlyStream<char> rs(std::string("abababa"));
    pc.ConsumeStream(rs);

    auto* res = pc.GetCounts();
    CHECK(pcCount(res, "ab") == 3, "PC(stream abababa): ab = 3");
    CHECK(pcCount(res, "ba") == 3, "PC(stream abababa): ba = 3");
    delete res;
}

static void test_pc_from_lazy_million()
{
    MutableArraySequence<std::string> pats;
    pats.Append(std::string("ab"));
    PatternCounter pc(pats, true);

    auto rule = [](const BoundedQueue<char>& q) -> char {
        return q.Back() == 'a' ? 'b' : 'a';
    };
    MutableArraySequence<char> init;
    init.Append('a');
    Generator<char> gen(rule, init, 1, 1000000);
    LazySequence<char> lz(gen);

    pc.ConsumeLazy(lz);
    auto* res = pc.GetCounts();
    CHECK(pcCount(res, "ab") == 500000, "PC(lazy abab... 1M символов): ab = 500000");
    CHECK(pc.GetProcessedCount() == 1000000, "PC: обработано 1M символов");
    delete res;
}

static void test_kmp_basic()
{
    KmpCounter k("ana", true);
    for (char c : std::string("banana")) k.ConsumeChar(c);
    CHECK(k.GetCount() == 2, "KMP(ana в banana, overlap=ON): 2");

    KmpCounter k2("ana", false);
    for (char c : std::string("banana")) k2.ConsumeChar(c);
    CHECK(k2.GetCount() == 1, "KMP(ana в banana, overlap=OFF): 1");
}

static void test_kmp_overlap()
{
    KmpCounter k("aba", true);
    for (char c : std::string("ababababa")) k.ConsumeChar(c);
    CHECK(k.GetCount() == 4, "KMP(aba в ababababa, overlap=ON): 4");

    KmpCounter k2("aba", false);
    for (char c : std::string("ababababa")) k2.ConsumeChar(c);
    CHECK(k2.GetCount() == 2, "KMP(aba в ababababa, overlap=OFF): 2");
}

int main()
{
    setlocale(LC_ALL, "Rus");

    smoke_sequence();
    test_optional();
    test_cardinal();
    test_bounded_queue();
    test_generator_naturals();
    test_generator_fibonacci();
    test_generator_finite_bound();
    test_generator_insert_at();
    test_generator_remove_at();
    test_generator_insert_subseq();
    test_generator_try_get_next();
    test_lazy_from_sequence();
    test_lazy_from_generator_finite();
    test_lazy_from_generator_infinite();
    test_lazy_map_reduce();
    test_lazy_where();
    test_lazy_zip();
    test_lazy_append_concat();
    test_ro_stream_from_sequence();
    test_ro_stream_from_string();
    test_ro_stream_from_lazy_infinite();
    test_ro_stream_from_istream();
    test_ro_stream_seek_and_eos();
    test_wo_stream_to_sequence();
    test_wo_stream_to_ostream();
    test_pc_classic_ushers();
    test_pc_empty_input();
    test_pc_overlap_on();
    test_pc_overlap_off();
    test_pc_from_stream();
    test_pc_from_lazy_million();
    test_kmp_basic();
    test_kmp_overlap();

    std::cout << "\n=== Итог: " << (g_total - g_failed) << "/" << g_total
              << " тестов пройдено ===\n";
    return g_failed == 0 ? 0 : 1;
}
