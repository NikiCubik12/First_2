#include <cctype>
#include <string>
#include <cstring>
#include <cstdlib>

#if __has_include(<ncurses/ncurses.h>)
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "Sequence.hpp"

// Константы для кнопок
constexpr int MAIN_BUTTON_COUNT = 3;
constexpr int DYNAMIC_ARRAY_BUTTON_COUNT = 8;
constexpr int LINKED_LIST_BUTTON_COUNT = 13;
constexpr int SEQUENCE_TYPE_BUTTON_COUNT = 3;
constexpr int ARRAY_SEQUENCE_IMPL_BUTTON_COUNT = 3;  // Mutable, Immutable, Back
constexpr int SEQUENCE_ACTION_BUTTON_COUNT = 9;

enum class Screen {
    MainMenu,
    DynamicArray,
    LinkedList,
    SequenceType,
    ArraySequenceImpl,  // Для выбора Mutable/Immutable для ArraySequence
    SequenceActions,
    Exit
};

enum class SequenceType {
    ArraySequence,
    ListSequence,
    BitSequence
};

enum class ArraySequenceImplType {
    Mutable,
    Immutable,
    None
};

struct Button {
    const char* label;
    int y;
    int x;
    int width;
};

// Глобальные переменные для Sequence
SequenceType currentSeqType = SequenceType::ArraySequence;
ArraySequenceImplType currentArraySeqImpl = ArraySequenceImplType::None;
Sequence<int>* intSequence = nullptr;
Sequence<int>* bitSequence = nullptr;
bool sequenceCreated = false;
std::string sequenceMessage = "Create sequence first.";

// Прототипы функций
void drawButton(const Button& button, bool focused);
bool buttonContains(const Button& button, int y, int x);
void drawMainMenu(const Button buttons[MAIN_BUTTON_COUNT], int selected);
Screen selectMainMenuItem(int selected);

// Ввод данных
bool readIntFromUser(int& value);
bool readBoolFromUser(bool& value);
bool readIndexFromUser(int& index);

// DynamicArray функции
void drawDynamicArrayScreen(const Button buttons[DYNAMIC_ARRAY_BUTTON_COUNT], int selected, 
                            DynamicArray<int>& array, bool created, const std::string& message);
void drawDynamicArrayValues(DynamicArray<int>& array, bool created);
void handleDynamicArrayAction(int selected, Screen& screen, DynamicArray<int>& array, 
                              bool& created, std::string& message);

// LinkedList функции
void drawLinkedListScreen(const Button buttons[LINKED_LIST_BUTTON_COUNT], int selected, 
                          LinkedList<int>& list, bool created, const std::string& message);
void drawLinkedListValues(LinkedList<int>& list, bool created);
void handleLinkedListAction(int selected, Screen& screen, LinkedList<int>& list, 
                            bool& created, std::string& message);

// Sequence функции
void drawSequenceTypeScreen(const Button buttons[SEQUENCE_TYPE_BUTTON_COUNT], int selected);
void drawArraySequenceImplScreen(const Button buttons[ARRAY_SEQUENCE_IMPL_BUTTON_COUNT], int selected);
void drawSequenceActionsScreen(const Button buttons[SEQUENCE_ACTION_BUTTON_COUNT], int selected, 
                               bool created, const std::string& message);
void drawSequenceValues();
void handleSequenceTypeAction(int selected, Screen& screen);
void handleArraySequenceImplAction(int selected, Screen& screen);
void handleSequenceAction(int selected, Screen& screen, std::string& message);
void createSequence();

// ==================== DYNAMIC ARRAY ====================
void drawDynamicArrayScreen(const Button buttons[DYNAMIC_ARRAY_BUTTON_COUNT], int selected, 
                            DynamicArray<int>& array, bool created, const std::string& message)
{
    erase();  // Очищаем весь экран
    mvprintw(0, 2, "=== DYNAMIC ARRAY ===");
    mvprintw(1, 2, "Use UP/DOWN to navigate, Enter to select, Backspace to menu, q to quit.");
    
    for (int i = 0; i < DYNAMIC_ARRAY_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }
    
    int statusY = 20;
    mvhline(statusY, 2, ACS_HLINE, COLS - 4);
    mvprintw(statusY + 1, 2, "Status: %s", message.c_str());
    mvprintw(statusY + 2, 2, "Created: %s | Size: %d", created ? "yes" : "no", created ? array.GetSize() : 0);
    
    drawDynamicArrayValues(array, created);
    refresh();
}

void drawDynamicArrayValues(DynamicArray<int>& array, bool created)
{
    int startY = 23;  // Фиксированная позиция
    int startX = 4;
    
    // Очищаем несколько строк, чтобы убрать артефакты
    for (int i = 0; i < 5; i++) {
        move(startY + i, 2);
        clrtoeol();
    }
    
    if (!created) {
        mvprintw(startY, startX, "(array is not created)");
    } else if (array.GetSize() == 0) {
        mvprintw(startY, startX, "[]");
    } else {
        std::string output = "Values: [";
        for (int i = 0; i < array.GetSize() && i < 30; i++) {
            if (i > 0) output += ", ";
            output += std::to_string(array.Get(i));
        }
        if (array.GetSize() > 30) output += ", ...";
        output += "]";
        mvprintw(startY, startX, "%s", output.c_str());
    }
}

void handleDynamicArrayAction(int selected, Screen& screen, DynamicArray<int>& array, 
                              bool& created, std::string& message)
{
    try {
        switch (selected) {
        case 0:  // Create
            array.Resize(0);
            created = true;
            message = "Dynamic array created.";
            break;
        case 1: {  // Append
            if (!created) { message = "Create array first."; break; }
            int value;
            if (readIntFromUser(value)) {
                array.Append(value);
                message = "Value appended.";
            } else {
                message = "Invalid integer value.";
            }
            break;
        }
        case 2: {  // Prepend
            if (!created) { message = "Create array first."; break; }
            int value;
            if (readIntFromUser(value)) {
                array.Prepend(value);
                message = "Value prepended.";
            } else {
                message = "Invalid integer value.";
            }
            break;
        }
        case 3: {  // Insert At
            if (!created) { message = "Create array first."; break; }
            int index, value;
            if (readIndexFromUser(index) && readIntFromUser(value)) {
                array.InsertAt(value, index);
                message = "Value inserted.";
            } else {
                message = "Invalid index or value.";
            }
            break;
        }
        case 4: {  // Set
            if (!created) { message = "Create array first."; break; }
            int index, value;
            if (readIndexFromUser(index) && readIntFromUser(value)) {
                array.Set(index, value);
                message = "Value set.";
            } else {
                message = "Invalid index or value.";
            }
            break;
        }
        case 5: {  // Get
            if (!created) { message = "Create array first."; break; }
            int index;
            if (readIndexFromUser(index)) {
                int value = array.Get(index);
                message = "Value at index " + std::to_string(index) + ": " + std::to_string(value);
            } else {
                message = "Invalid index.";
            }
            break;
        }
        case 6:  // Print
            if (!created) { message = "Create array first."; }
            else { message = "Array printed above."; }
            break;
        case 7:  // Back
            screen = Screen::MainMenu;
            break;
        default: break;
        }
    } catch (const std::out_of_range& e) {
        message = e.what();
    }
}

// ==================== LINKED LIST ====================
void drawLinkedListScreen(const Button buttons[LINKED_LIST_BUTTON_COUNT], int selected, 
                          LinkedList<int>& list, bool created, const std::string& message)
{
    erase();
    mvprintw(0, 2, "=== LINKED LIST ===");
    mvprintw(1, 2, "Use UP/DOWN to navigate, Enter to select, Backspace to menu, q to quit.");
    
    for (int i = 0; i < LINKED_LIST_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }
    
    int statusY = 6 + LINKED_LIST_BUTTON_COUNT;
    mvhline(statusY, 2, ACS_HLINE, COLS - 4);
    mvprintw(statusY + 1, 2, "Status: %s", message.c_str());
    mvprintw(statusY + 2, 2, "Created: %s | Size: %d", created ? "yes" : "no", created ? list.GetLength() : 0);
    
    drawLinkedListValues(list, created);
    refresh();
}

void drawLinkedListValues(LinkedList<int>& list, bool created)
{
    int startY = 18;
    int startX = 4;
    
    // Очищаем несколько строк
    for (int i = 0; i < 5; i++) {
        move(startY + i, 2);
        clrtoeol();
    }
    
    if (!created) {
        mvprintw(startY, startX, "(list is not created)");
    } else if (list.GetLength() == 0) {
        mvprintw(startY, startX, "[]");
    } else {
        std::string output = "Values: [";
        for (int i = 0; i < list.GetLength() && i < 30; i++) {
            if (i > 0) output += ", ";
            output += std::to_string(list.Get(i));
        }
        if (list.GetLength() > 30) output += ", ...";
        output += "]";
        mvprintw(startY, startX, "%s", output.c_str());
    }
}

void handleLinkedListAction(int selected, Screen& screen, LinkedList<int>& list, 
                            bool& created, std::string& message)
{
    try {
        switch (selected) {
        case 0:  // Create
            list.ClearList();
            created = true;
            message = "Linked list created.";
            break;
            
        case 1: {  // Append
            if (!created) { message = "Create list first."; break; }
            int value;
            if (readIntFromUser(value)) {
                list.Append(value);
                message = "Value appended.";
            } else {
                message = "Invalid integer value.";
            }
            break;
        }
        
        case 2: {  // Prepend
            if (!created) { message = "Create list first."; break; }
            int value;
            if (readIntFromUser(value)) {
                list.Prepend(value);
                message = "Value prepended.";
            } else {
                message = "Invalid integer value.";
            }
            break;
        }
        
        case 3: {  // Insert At
            if (!created) { message = "Create list first."; break; }
            int index, value;
            if (readIndexFromUser(index) && readIntFromUser(value)) {
                list.InsertAt(value, index);
                message = "Value inserted at index " + std::to_string(index);
            } else {
                message = "Invalid index or value.";
            }
            break;
        }
        
        case 4: {  // Get
            if (!created) { message = "Create list first."; break; }
            int index;
            if (readIndexFromUser(index)) {
                int value = list.Get(index);
                message = "Value at index " + std::to_string(index) + ": " + std::to_string(value);
            } else {
                message = "Invalid index.";
            }
            break;
        }
        
        case 5: {  // Get First
            if (!created) { message = "Create list first."; break; }
            int value = list.GetFirst();
            message = "First value: " + std::to_string(value);
            break;
        }
        
        case 6: {  // Get Last
            if (!created) { message = "Create list first."; break; }
            int value = list.GetLast();
            message = "Last value: " + std::to_string(value);
            break;
        }
        
        case 7: {  // Get SubList
            if (!created) { message = "Create list first."; break; }
            int start, end;
            mvprintw(22, 2, "Enter start index: ");
            echo();
            curs_set(1);
            char input[32];
            move(22, 20);
            getnstr(input, 31);
            start = atoi(input);
            mvprintw(23, 2, "Enter end index: ");
            move(23, 18);
            getnstr(input, 31);
            end = atoi(input);
            noecho();
            curs_set(0);
            LinkedList<int>* sublist = list.GetSubList(start, end);
            message = "Sublist created. Size: " + std::to_string(sublist->GetLength());
            // Выводим подсписок
            std::string out = "Sublist: [";
            for (int i = 0; i < sublist->GetLength() && i < 20; i++) {
                if (i > 0) out += ", ";
                out += std::to_string(sublist->Get(i));
            }
            out += "]";
            mvprintw(24, 2, "%s", out.c_str());
            delete sublist;
            break;
        }
        
        case 8: {  // Concat
            if (!created) { message = "Create list first."; break; }
            // Создаём второй список
            LinkedList<int>* list2 = new LinkedList<int>();
            mvprintw(22, 2, "Create second list for concatenation:");
            refresh();
            
            // Добавляем элементы во второй список
            int value;
            int count = 0;
            while (true) {
                mvprintw(23, 2, "Enter value (or non-integer to stop): ");
                echo();
                curs_set(1);
                char input[32];
                move(23, 40);
                getnstr(input, 31);
                noecho();
                curs_set(0);
                
                char* end = nullptr;
                long parsed = strtol(input, &end, 10);
                if (input[0] == '\0' || *end != '\0') {
                    break;
                }
                value = static_cast<int>(parsed);
                list2->Append(value);
                count++;
                
                mvprintw(24, 2, "Added: %d, Total: %d", value, count);
                refresh();
            }
            
            LinkedList<int>* result = list.Concat(list2);
            message = "Concatenation result created. Size: " + std::to_string(result->GetLength());
            std::string out = "Result: [";
            for (int i = 0; i < result->GetLength() && i < 20; i++) {
                if (i > 0) out += ", ";
                out += std::to_string(result->Get(i));
            }
            out += "]";
            mvprintw(25, 2, "%s", out.c_str());
            delete list2;
            delete result;
            break;
        }
        
        case 9:  // Get Length
            if (!created) { message = "Create list first."; }
            else { message = "List length: " + std::to_string(list.GetLength()); }
            break;
        
        case 10:  // Print
            if (!created) { message = "Create list first."; }
            else { message = "List printed above."; }
            break;
        
        case 11:  // Clear
            if (!created) { message = "Create list first."; break; }
            list.ClearList();
            message = "List cleared.";
            break;
        
        case 12:  // Back
            screen = Screen::MainMenu;
            break;
        
        default: break;
        }
    } catch (const std::out_of_range& e) {
        message = e.what();
    }
}

// ==================== SEQUENCE ====================
void drawSequenceTypeScreen(const Button buttons[SEQUENCE_TYPE_BUTTON_COUNT], int selected)
{
    erase();
    mvprintw(0, 2, "=== SELECT SEQUENCE TYPE ===");
    mvprintw(1, 2, "Use UP/DOWN to navigate, Enter to select, Backspace to return.");
    
    // Рисуем кнопки вручную
    const char* options[3] = {"ArraySequence", "ListSequence", "BitSequence"};
    int y = 4;
    for (int i = 0; i < 3; i++) {
        if (i == selected) attron(A_REVERSE);
        mvprintw(y + i * 2, 4, "[ %-20s ]", options[i]);
        if (i == selected) attroff(A_REVERSE);
    }
    
    mvprintw(12, 2, "Choose which type of sequence to use:");
    mvprintw(13, 4, "ArraySequence - based on DynamicArray (supports Mutable/Immutable)");
    mvprintw(14, 4, "ListSequence - based on LinkedList");
    mvprintw(15, 4, "BitSequence - sequence of boolean values");
    refresh();
}

void drawArraySequenceImplScreen(const Button buttons[ARRAY_SEQUENCE_IMPL_BUTTON_COUNT], int selected)
{
    erase();
    mvprintw(0, 2, "=== SELECT ARRAY SEQUENCE IMPLEMENTATION ===");
    mvprintw(1, 2, "Use UP/DOWN to navigate, Enter to select, Backspace to return.");
    
    const char* options[3] = {"Mutable", "Immutable", "Back"};
    int y = 4;
    for (int i = 0; i < 3; i++) {
        if (i == selected) attron(A_REVERSE);
        mvprintw(y + i * 2, 4, "[ %-20s ]", options[i]);
        if (i == selected) attroff(A_REVERSE);
    }
    
    mvprintw(12, 2, "Choose mutable or immutable variant:");
    mvprintw(13, 4, "Mutable - operations modify the original sequence");
    mvprintw(14, 4, "Immutable - operations return a new sequence (original unchanged)");
    refresh();
}

void drawSequenceActionsScreen(const Button buttons[SEQUENCE_ACTION_BUTTON_COUNT], int selected, 
                               bool created, const std::string& message)
{
    erase();
    mvprintw(0, 2, "=== SEQUENCE OPERATIONS ===");
    mvprintw(1, 2, "Use UP/DOWN to navigate, Enter to select, Backspace to return, q to quit.");
    
    for (int i = 0; i < SEQUENCE_ACTION_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }
    
    int statusY = 11 + SEQUENCE_ACTION_BUTTON_COUNT;
    mvhline(statusY, 2, ACS_HLINE, COLS - 4);
    mvprintw(statusY + 1, 2, "Status: %s", message.c_str());
    
    std::string typeStr = (currentSeqType == SequenceType::ArraySequence) ? "ArraySequence" :
                          (currentSeqType == SequenceType::ListSequence) ? "ListSequence" : "BitSequence";
    if (currentSeqType == SequenceType::ArraySequence && currentArraySeqImpl == ArraySequenceImplType::Mutable)
        typeStr += " (Mutable)";
    else if (currentSeqType == SequenceType::ArraySequence && currentArraySeqImpl == ArraySequenceImplType::Immutable)
        typeStr += " (Immutable)";
    
    mvprintw(statusY + 2, 2, "Sequence: %s | Created: %s | Size: %d", 
             typeStr.c_str(), created ? "yes" : "no", 
             created ? (currentSeqType == SequenceType::BitSequence ? bitSequence->GetLength() : intSequence->GetLength()) : 0);
    
    drawSequenceValues();
    refresh();
}

void drawSequenceValues()
{
    int startY = 22;
    int startX = 4;
    
    // Очищаем несколько строк
    for (int i = 0; i < 5; i++) {
        move(startY + i, 2);
        clrtoeol();
    }
    
    mvprintw(startY - 1, 2, "Values:");
    
    if (!sequenceCreated) {
        mvprintw(startY, startX, "(sequence is not created)");
        return;
    }
    
    std::string output = "[";
    if (currentSeqType == SequenceType::BitSequence) {
        for (int i = 0; i < bitSequence->GetLength() && i < 30; i++) {
            if (i > 0) output += ", ";
            output += std::to_string(bitSequence->Get(i));
        }
    } else {
        for (int i = 0; i < intSequence->GetLength() && i < 30; i++) {
            if (i > 0) output += ", ";
            output += std::to_string(intSequence->Get(i));
        }
    }
    if ((currentSeqType == SequenceType::BitSequence ? bitSequence->GetLength() : intSequence->GetLength()) > 30) {
        output += ", ...";
    }
    output += "]";
    
    mvprintw(startY, startX, "%s", output.c_str());
}

void createSequence()
{
    if (intSequence) {
        delete intSequence;
        intSequence = nullptr;
    }
    if (bitSequence) {
        delete bitSequence;
        bitSequence = nullptr;
    }
    
    try {
        if (currentSeqType == SequenceType::BitSequence) {
            bitSequence = new BitSequence();
            sequenceCreated = true;
        } 
        else if (currentSeqType == SequenceType::ArraySequence) {
            if (currentArraySeqImpl == ArraySequenceImplType::Mutable) {
                intSequence = new MutableArraySequence<int>();
            } else if (currentArraySeqImpl == ArraySequenceImplType::Immutable) {
                intSequence = new ImmutableArraySequence<int>();
            } else {
                return;
            }
            sequenceCreated = true;
        }
        else if (currentSeqType == SequenceType::ListSequence) {
            intSequence = new ListSequence<int>();
            sequenceCreated = true;
        }
        sequenceMessage = "Sequence created.";
    } catch (const std::exception& e) {
        sequenceMessage = std::string("Error: ") + e.what();
        sequenceCreated = false;
    }
}

void handleSequenceTypeAction(int selected, Screen& screen)
{
    switch (selected) {
    case 0:
        currentSeqType = SequenceType::ArraySequence;
        screen = Screen::ArraySequenceImpl;
        break;
    case 1:
        currentSeqType = SequenceType::ListSequence;
        currentArraySeqImpl = ArraySequenceImplType::None;
        createSequence();
        screen = Screen::SequenceActions;
        break;
    case 2:
        currentSeqType = SequenceType::BitSequence;
        currentArraySeqImpl = ArraySequenceImplType::None;
        createSequence();
        screen = Screen::SequenceActions;
        break;
    }
}

void handleArraySequenceImplAction(int selected, Screen& screen)
{
    switch (selected) {
    case 0:
        currentArraySeqImpl = ArraySequenceImplType::Mutable;
        break;
    case 1:
        currentArraySeqImpl = ArraySequenceImplType::Immutable;
        break;
    case 2:
        screen = Screen::SequenceType;
        return;
    }
    createSequence();
    screen = Screen::SequenceActions;
}

void handleSequenceAction(int selected, Screen& screen, std::string& message)
{
    if (!sequenceCreated && selected != 0 && selected != 8) {
        message = "Create sequence first!";
        return;
    }
    
    try {
        switch (selected) {
        case 0:  // Create
            createSequence();
            message = sequenceCreated ? "Sequence created." : "Failed to create sequence.";
            break;
            
        case 1: {  // Append
            if (currentSeqType == SequenceType::BitSequence) {
                bool val;
                if (readBoolFromUser(val)) {
                    bitSequence->Append(val ? 1 : 0);
                    message = "Value appended.";
                } else {
                    message = "Invalid value (0 or 1).";
                }
            } else {
                int val;
                if (readIntFromUser(val)) {
                    intSequence->Append(val);
                    message = "Value appended.";
                } else {
                    message = "Invalid integer value.";
                }
            }
            break;
        }
            
        case 2: {  // Prepend
            if (currentSeqType == SequenceType::BitSequence) {
                bool val;
                if (readBoolFromUser(val)) {
                    bitSequence->Prepend(val ? 1 : 0);
                    message = "Value prepended.";
                } else {
                    message = "Invalid value (0 or 1).";
                }
            } else {
                int val;
                if (readIntFromUser(val)) {
                    intSequence->Prepend(val);
                    message = "Value prepended.";
                } else {
                    message = "Invalid integer value.";
                }
            }
            break;
        }
            
        case 3: {  // Insert At
            int index;
            if (!readIndexFromUser(index)) {
                message = "Invalid index.";
                break;
            }
            if (currentSeqType == SequenceType::BitSequence) {
                bool val;
                if (readBoolFromUser(val)) {
                    bitSequence->InsertAt(val ? 1 : 0, index);
                    message = "Value inserted.";
                } else {
                    message = "Invalid value (0 or 1).";
                }
            } else {
                int val;
                if (readIntFromUser(val)) {
                    intSequence->InsertAt(val, index);
                    message = "Value inserted.";
                } else {
                    message = "Invalid integer value.";
                }
            }
            break;
        }
            
        case 4: {  // Get
            int index;
            if (!readIndexFromUser(index)) {
                message = "Invalid index.";
                break;
            }
            if (currentSeqType == SequenceType::BitSequence) {
                int val = bitSequence->Get(index);
                message = "Value at index " + std::to_string(index) + ": " + std::to_string(val);
            } else {
                int val = intSequence->Get(index);
                message = "Value at index " + std::to_string(index) + ": " + std::to_string(val);
            }
            break;
        }
            
        case 5: {  // Get First
            if (currentSeqType == SequenceType::BitSequence) {
                int val = bitSequence->GetFirst();
                message = "First value: " + std::to_string(val);
            } else {
                int val = intSequence->GetFirst();
                message = "First value: " + std::to_string(val);
            }
            break;
        }
            
        case 6: {  // Get Last
            if (currentSeqType == SequenceType::BitSequence) {
                int val = bitSequence->GetLast();
                message = "Last value: " + std::to_string(val);
            } else {
                int val = intSequence->GetLast();
                message = "Last value: " + std::to_string(val);
            }
            break;
        }
            
        case 7:  // Print
            message = "Values printed above.";
            break;
            
        case 8:  // Back
            screen = Screen::MainMenu;
            break;
        }
    } catch (const std::out_of_range& e) {
        message = std::string("Index out of range: ") + e.what();
    } catch (const std::invalid_argument& e) {
        message = e.what();
    } catch (const std::exception& e) {
        message = e.what();
    }
    
    noecho();
    curs_set(0);
}

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================
bool readIntFromUser(int& value)
{
    char input[32];
    mvprintw(22, 2, "Enter integer value: ");
    clrtoeol();
    echo();
    curs_set(1);
    move(22, 23);
    getnstr(input, sizeof(input) - 1);
    noecho();
    curs_set(0);
    char* end = nullptr;
    long parsed = strtol(input, &end, 10);
    if (input[0] == '\0' || *end != '\0') {
        return false;
    }
    value = static_cast<int>(parsed);
    return true;
}

bool readBoolFromUser(bool& value)
{
    char input[5];
    mvprintw(22, 2, "Enter value (0 or 1): ");
    clrtoeol();
    echo();
    curs_set(1);
    move(22, 25);
    getnstr(input, sizeof(input) - 1);
    noecho();
    curs_set(0);
    if (input[0] == '0') value = false;
    else if (input[0] == '1') value = true;
    else return false;
    return true;
}

bool readIndexFromUser(int& index)
{
    char input[32];
    mvprintw(22, 2, "Enter index: ");
    clrtoeol();
    echo();
    curs_set(1);
    move(22, 14);
    getnstr(input, sizeof(input) - 1);
    noecho();
    curs_set(0);
    char* end = nullptr;
    long parsed = strtol(input, &end, 10);
    if (input[0] == '\0' || *end != '\0' || parsed < 0) {
        return false;
    }
    index = static_cast<int>(parsed);
    return true;
}

void drawButton(const Button& button, bool focused)
{
    if (focused) attron(A_REVERSE);
    mvprintw(button.y, button.x, "[ %-*s ]", button.width - 4, button.label);
    if (focused) attroff(A_REVERSE);
}

bool buttonContains(const Button& button, int y, int x)
{
    return y == button.y && x >= button.x && x < button.x + button.width;
}

void drawMainMenu(const Button buttons[MAIN_BUTTON_COUNT], int selected)
{
    erase();
    mvprintw(0, 2, "=== MAIN MENU ===");
    mvprintw(1, 2, "Use UP/DOWN to navigate, Enter to select, q to quit.");
    for (int i = 0; i < MAIN_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }
    refresh();
}

Screen selectMainMenuItem(int selected)
{
    switch (selected) {
    case 0: return Screen::DynamicArray;
    case 1: return Screen::LinkedList;
    case 2: return Screen::SequenceType;
    default: return Screen::MainMenu;
    }
}

int main()
{   
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    mousemask(BUTTON1_CLICKED, nullptr);

    Screen screen = Screen::MainMenu;
    int selected = 0;
    int dynamicArraySelected = 0;
    int linkedListSelected = 0;
    int seqTypeSelected = 0;
    int seqImplSelected = 0;
    int seqActionSelected = 0;

    DynamicArray<int> dynamicArray;
    bool dynamicArrayCreated = false;
    std::string dynamicArrayMessage = "Create dynamic array to begin.";

    LinkedList<int> linkedList;
    bool linkedListCreated = false;
    std::string linkedListMessage = "Create linked list to begin.";

    // Главное меню (вертикальное)
    Button mainButtons[MAIN_BUTTON_COUNT] = {
        {"Dynamic Array", 4, 2, 20},
        {"Linked List", 6, 2, 20},
        {"Sequence", 8, 2, 20},
    };

    // Dynamic Array кнопки (вертикальные)
    Button dynamicArrayButtons[DYNAMIC_ARRAY_BUTTON_COUNT] = {
        {"Create", 4, 2, 16},
        {"Append", 6, 2, 16},
        {"Prepend", 8, 2, 16},
        {"Insert At", 10, 2, 16},
        {"Set", 12, 2, 16},
        {"Get", 14, 2, 16},
        // {"Resize", 16, 2, 16},  
        {"Print", 16, 2, 16},
        {"Back", 18, 2, 16},
    };

    // Linked List кнопки (вертикальные)
    Button linkedListButtons[LINKED_LIST_BUTTON_COUNT] = {
        {"Create", 4, 2, 16},
        {"Append", 6, 2, 16},
        {"Prepend", 8, 2, 16},
        {"Insert At", 10, 2, 16},
        {"Get", 12, 2, 16},
        {"Get First", 14, 2, 16},
        {"Get Last", 16, 2, 16},
        {"Get SubList", 18, 2, 16},
        {"Concat", 20, 2, 16},
        {"Get Length", 22, 2, 16},
        {"Print", 24, 2, 16},
        {"Clear", 26, 2, 16},
        {"Back", 28, 2, 16},
    };

    // Sequence Type кнопки (вертикальные)
    Button seqTypeButtons[SEQUENCE_TYPE_BUTTON_COUNT] = {
        {"ArraySequence", 4, 2, 20},
        {"ListSequence", 6, 2, 20},
        {"BitSequence", 8, 2, 20},
    };

    // ArraySequence Implementation кнопки (вертикальные)
    Button seqImplButtons[ARRAY_SEQUENCE_IMPL_BUTTON_COUNT] = {
        {"Mutable", 4, 2, 20},
        {"Immutable", 6, 2, 20},
        {"Back", 8, 2, 20},
    };

    // Sequence Actions кнопки (вертикальные)
    Button seqActionButtons[SEQUENCE_ACTION_BUTTON_COUNT] = {
        {"Create", 4, 2, 16},
        {"Append", 6, 2, 16},
        {"Prepend", 8, 2, 16},
        {"Insert At", 10, 2, 16},
        {"Get", 12, 2, 16},
        {"Get First", 14, 2, 16},
        {"Get Last", 16, 2, 16},
        {"Print", 18, 2, 16},
        {"Back", 20, 2, 16},
    };

    while (screen != Screen::Exit) {
        if (screen == Screen::MainMenu) {
            drawMainMenu(mainButtons, selected);
            int ch = getch();
            if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            } else if (ch == KEY_UP) {
                selected = (selected + MAIN_BUTTON_COUNT - 1) % MAIN_BUTTON_COUNT;
            } else if (ch == KEY_DOWN || ch == '\t') {
                selected = (selected + 1) % MAIN_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                screen = selectMainMenuItem(selected);
            } else if (ch == KEY_MOUSE) {
                MEVENT event;
                if (getmouse(&event) == OK) {
                    for (int i = 0; i < MAIN_BUTTON_COUNT; ++i) {
                        if (buttonContains(mainButtons[i], event.y, event.x)) {
                            selected = i;
                            screen = selectMainMenuItem(selected);
                            break;
                        }
                    }
                }
            }
        }
        else if (screen == Screen::DynamicArray) {
            drawDynamicArrayScreen(dynamicArrayButtons, dynamicArraySelected, dynamicArray, 
                                   dynamicArrayCreated, dynamicArrayMessage);
            int ch = getch();
            if (ch == KEY_UP) {
                dynamicArraySelected = (dynamicArraySelected + DYNAMIC_ARRAY_BUTTON_COUNT - 1) % DYNAMIC_ARRAY_BUTTON_COUNT;
            } else if (ch == KEY_DOWN || ch == '\t') {
                dynamicArraySelected = (dynamicArraySelected + 1) % DYNAMIC_ARRAY_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleDynamicArrayAction(dynamicArraySelected, screen, dynamicArray, 
                                        dynamicArrayCreated, dynamicArrayMessage);
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::MainMenu;
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
        else if (screen == Screen::LinkedList) {
            drawLinkedListScreen(linkedListButtons, linkedListSelected, linkedList, 
                                 linkedListCreated, linkedListMessage);
            int ch = getch();
            if (ch == KEY_UP) {
                linkedListSelected = (linkedListSelected + LINKED_LIST_BUTTON_COUNT - 1) % LINKED_LIST_BUTTON_COUNT;
            } else if (ch == KEY_DOWN || ch == '\t') {
                linkedListSelected = (linkedListSelected + 1) % LINKED_LIST_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleLinkedListAction(linkedListSelected, screen, linkedList, 
                                       linkedListCreated, linkedListMessage);
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::MainMenu;
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
        else if (screen == Screen::SequenceType) {
            drawSequenceTypeScreen(seqTypeButtons, seqTypeSelected);
            int ch = getch();
            if (ch == KEY_UP) {
                seqTypeSelected = (seqTypeSelected + SEQUENCE_TYPE_BUTTON_COUNT - 1) % SEQUENCE_TYPE_BUTTON_COUNT;
            } else if (ch == KEY_DOWN || ch == '\t') {
                seqTypeSelected = (seqTypeSelected + 1) % SEQUENCE_TYPE_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleSequenceTypeAction(seqTypeSelected, screen);
                seqTypeSelected = 0;
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::MainMenu;
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
        else if (screen == Screen::ArraySequenceImpl) {
            drawArraySequenceImplScreen(seqImplButtons, seqImplSelected);
            int ch = getch();
            if (ch == KEY_UP) {
                seqImplSelected = (seqImplSelected + ARRAY_SEQUENCE_IMPL_BUTTON_COUNT - 1) % ARRAY_SEQUENCE_IMPL_BUTTON_COUNT;
            } else if (ch == KEY_DOWN || ch == '\t') {
                seqImplSelected = (seqImplSelected + 1) % ARRAY_SEQUENCE_IMPL_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleArraySequenceImplAction(seqImplSelected, screen);
                seqImplSelected = 0;
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::SequenceType;
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
        else if (screen == Screen::SequenceActions) {
            drawSequenceActionsScreen(seqActionButtons, seqActionSelected, sequenceCreated, sequenceMessage);
            int ch = getch();
            if (ch == KEY_UP) {
                seqActionSelected = (seqActionSelected + SEQUENCE_ACTION_BUTTON_COUNT - 1) % SEQUENCE_ACTION_BUTTON_COUNT;
            } else if (ch == KEY_DOWN || ch == '\t') {
                seqActionSelected = (seqActionSelected + 1) % SEQUENCE_ACTION_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleSequenceAction(seqActionSelected, screen, sequenceMessage);
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                if (currentSeqType == SequenceType::ArraySequence) {
                    screen = Screen::ArraySequenceImpl;
                } else {
                    screen = Screen::SequenceType;
                }
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
    }

    if (intSequence) delete intSequence;
    if (bitSequence) delete bitSequence;
    endwin();
    return 0;
}