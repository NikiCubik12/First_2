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
constexpr int SEQUENCE_TYPE_BUTTON_COUNT = 3;
constexpr int SEQUENCE_IMPL_BUTTON_COUNT = 3;
constexpr int SEQUENCE_ACTION_BUTTON_COUNT = 6;
constexpr int DYNAMIC_ARRAY_BUTTON_COUNT = 5;
constexpr int LINKED_LIST_BUTTON_COUNT = 6;

enum class Screen {
    MainMenu,
    DynamicArray,
    LinkedList,
    SequenceType,
    SequenceImpl,
    SequenceActions,
    Exit
};

enum class SequenceType {
    ArraySequence,
    ListSequence,
    BitSequence
};

enum class SequenceImplType {
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
SequenceImplType currentSeqImpl = SequenceImplType::None;
Sequence<int>* intSequence = nullptr;
Sequence<bool>* bitSequence = nullptr;
bool sequenceCreated = false;
std::string sequenceMessage = "Create sequence first.";

// Прототипы функций
void drawButton(const Button& button, bool focused);
bool buttonContains(const Button& button, int y, int x);
void drawMainMenu(const Button buttons[MAIN_BUTTON_COUNT], int selected);
Screen selectMainMenuItem(int selected);
bool readIntFromUser(int& value);
bool readBoolFromUser(bool& value);

// DynamicArray функции
void drawDynamicArrayScreen(const Button buttons[DYNAMIC_ARRAY_BUTTON_COUNT], int selected, DynamicArray<int>& array, bool created, const std::string& message);
void drawDynamicArrayValues(DynamicArray<int>& array, bool created);
void handleDynamicArrayAction(int selected, Screen& screen, DynamicArray<int>& array, bool& created, std::string& message);

// LinkedList функции
void drawLinkedListScreen(const Button buttons[LINKED_LIST_BUTTON_COUNT], int selected, LinkedList<int>& list, bool created, const std::string& message);
void drawLinkedListValues(LinkedList<int>& list, bool created);
void handleLinkedListAction(int selected, Screen& screen, LinkedList<int>& list, bool& created, std::string& message);

// Sequence функции
void drawSequenceTypeScreen(const Button buttons[SEQUENCE_TYPE_BUTTON_COUNT], int selected);
void drawSequenceImplScreen(const Button buttons[SEQUENCE_IMPL_BUTTON_COUNT], int selected);
void drawSequenceActionsScreen(const Button buttons[SEQUENCE_ACTION_BUTTON_COUNT], int selected, bool created, const std::string& message);
void drawSequenceValues();
void handleSequenceTypeAction(int selected, Screen& screen);
void handleSequenceImplAction(int selected, Screen& screen);
void handleSequenceAction(int selected, Screen& screen, std::string& message);
void createSequence();

void drawPlaceholderScreen(const char* title)
{
    erase();
    mvprintw(1, 2, "%s", title);
    mvprintw(3, 2, "This screen will be implemented later.");
    mvprintw(5, 2, "Press Backspace to return.");
    mvprintw(6, 2, "Press q to quit.");
    refresh();
}

// ==================== DYNAMIC ARRAY ====================
void drawDynamicArrayScreen(const Button buttons[DYNAMIC_ARRAY_BUTTON_COUNT], int selected, DynamicArray<int>& array, bool created, const std::string& message)
{
    erase();
    mvprintw(1, 2, "Dynamic Array");
    mvprintw(2, 2, "Use Left/Right or Tab, Enter/Space to select, Backspace to menu, q to quit.");

    for (int i = 0; i < DYNAMIC_ARRAY_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }

    mvhline(7, 2, ACS_HLINE, COLS - 4);
    mvprintw(8, 2, "Status: %s", message.c_str());
    mvprintw(9, 2, "Created: %s | Size: %d", created ? "yes" : "no", created ? array.GetSize() : 0);

    if (!created) {
        mvprintw(11, 4, "(array is not created)");
    } else if (array.GetSize() == 0) {
        mvprintw(11, 4, "[]");
    } else {
        mvprintw(11, 2, "Values: [");
        int x = 10;
        for (int i = 0; i < array.GetSize() && i < 20; i++) {
            mvprintw(11, x, "%d", array.Get(i));
            x += 4;
            if (i < array.GetSize() - 1) {
                mvprintw(11, x - 2, ",");
            }
        }
        if (array.GetSize() > 20) mvprintw(11, x, "...");
        mvprintw(11, x + 4, "]");
    }
    refresh();
}

void drawLinkedListScreen(const Button buttons[LINKED_LIST_BUTTON_COUNT], int selected, LinkedList<int>& list, bool created, const std::string& message)
{
    erase();
    mvprintw(1, 2, "Linked List");
    mvprintw(2, 2, "Use Left/Right or Tab, Enter/Space to select, Backspace to menu, q to quit.");

    for (int i = 0; i < LINKED_LIST_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }

    mvhline(7, 2, ACS_HLINE, COLS - 4);
    mvprintw(8, 2, "Status: %s", message.c_str());
    mvprintw(9, 2, "Created: %s | Size: %d", created ? "yes" : "no", created ? list.GetLength() : 0);

    if (!created) {
        mvprintw(11, 4, "(list is not created)");
    } else if (list.GetLength() == 0) {
        mvprintw(11, 4, "[]");
    } else {
        mvprintw(11, 2, "Values: [");
        int x = 10;
        for (int i = 0; i < list.GetLength() && i < 20; i++) {
            mvprintw(11, x, "%d", list.Get(i));
            x += 4;
            if (i < list.GetLength() - 1) {
                mvprintw(11, x - 2, ",");
            }
        }
        if (list.GetLength() > 20) mvprintw(11, x, "...");
        mvprintw(11, x + 4, "]");
    }
    refresh();
}

void drawSequenceTypeScreen(const Button buttons[SEQUENCE_TYPE_BUTTON_COUNT], int selected)
{
    erase();
    mvprintw(1, 2, "Select Sequence Type");
    mvprintw(2, 2, "Use Left/Right or Tab, Enter/Space to select, Backspace to return.");

    for (int i = 0; i < SEQUENCE_TYPE_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }
    
    mvprintw(7, 2, "Choose which type of sequence to use:");
    mvprintw(8, 4, "ArraySequence - based on DynamicArray");
    mvprintw(9, 4, "ListSequence - based on LinkedList");
    mvprintw(10, 4, "BitSequence - sequence of boolean values");
    refresh();
}

void drawSequenceImplScreen(const Button buttons[SEQUENCE_IMPL_BUTTON_COUNT], int selected)
{
    erase();
    mvprintw(1, 2, "Select Implementation Type");
    mvprintw(2, 2, "Use Left/Right or Tab, Enter/Space to select, Backspace to return.");

    for (int i = 0; i < SEQUENCE_IMPL_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }
    
    mvprintw(7, 2, "Choose mutable or immutable:");
    mvprintw(8, 4, "Mutable - operations modify the original sequence");
    mvprintw(9, 4, "Immutable - operations return a new sequence");
    refresh();
}

void drawSequenceActionsScreen(const Button buttons[SEQUENCE_ACTION_BUTTON_COUNT], int selected, bool created, const std::string& message)
{
    erase();
    mvprintw(1, 2, "Sequence Operations");
    mvprintw(2, 2, "Use Left/Right or Tab, Enter/Space to select, Backspace to return, q to quit.");

    for (int i = 0; i < SEQUENCE_ACTION_BUTTON_COUNT; ++i) {
        drawButton(buttons[i], i == selected);
    }

    mvhline(9, 2, ACS_HLINE, COLS - 4);
    mvprintw(10, 2, "Status: %s", message.c_str());
    mvprintw(11, 2, "Created: %s | Size: %d", created ? "yes" : "no", created ? (currentSeqType == SequenceType::BitSequence ? bitSequence->GetLength() : intSequence->GetLength()) : 0);
    
    drawSequenceValues();
    refresh();
}

void drawSequenceValues()
{
    mvprintw(12, 2, "Current values:");
    if (!sequenceCreated) {
        mvprintw(13, 4, "(sequence is not created)");
        return;
    }
    
    if (currentSeqType == SequenceType::BitSequence) {
        if (bitSequence->GetLength() == 0) {
            mvprintw(13, 4, "[]");
        } else {
            mvprintw(13, 2, "[");
            int x = 3;
            for (int i = 0; i < bitSequence->GetLength() && i < 30; i++) {
                mvprintw(13, x, "%d", bitSequence->Get(i));
                x += 2;
            }
            mvprintw(13, x, "]");
        }
    } else {
        if (intSequence->GetLength() == 0) {
            mvprintw(13, 4, "[]");
        } else {
            mvprintw(13, 2, "[");
            int x = 3;
            for (int i = 0; i < intSequence->GetLength() && i < 30; i++) {
                mvprintw(13, x, "%d", intSequence->Get(i));
                x += 4;
            }
            mvprintw(13, x, "]");
        }
    }
}

void handleDynamicArrayAction(int selected, Screen& screen, DynamicArray<int>& array, bool& created, std::string& message)
{
    switch (selected) {
    case 0:
        array.Resize(0);
        created = true;
        message = "Dynamic array created.";
        break;
    case 1: {
        if (!created) {
            message = "Create dynamic array first.";
            break;
        }
        int value;
        if (readIntFromUser(value)) {
            array.Append(value);
            message = "Value added.";
        } else {
            message = "Invalid integer value.";
        }
        break;
    }
    case 2: {
        if (!created) {
            message = "Create dynamic array first.";
            break;
        }
        int index, value;
        mvprintw(15, 2, "Enter index: ");
        echo();
        curs_set(1);
        char input[32];
        move(15, 15);
        getnstr(input, 31);
        index = atoi(input);
        mvprintw(16, 2, "Enter value: ");
        move(16, 15);
        getnstr(input, 31);
        value = atoi(input);
        noecho();
        curs_set(0);
        try {
            array.Set(index, value);
            message = "Value set.";
        } catch (...) {
            message = "Invalid index!";
        }
        break;
    }
    case 3:
        if (!created) {
            message = "Create dynamic array first.";
        } else {
            message = "Array printed above.";
        }
        break;
    case 4:
        screen = Screen::MainMenu;
        break;
    default:
        break;
    }
}

void handleLinkedListAction(int selected, Screen& screen, LinkedList<int>& list, bool& created, std::string& message)
{
    switch (selected) {
    case 0:
        list.ClearList();
        created = true;
        message = "Linked list created.";
        break;
    case 1: {
        if (!created) {
            message = "Create linked list first.";
            break;
        }
        int value;
        if (readIntFromUser(value)) {
            list.Append(value);
            message = "Value appended.";
        } else {
            message = "Invalid integer value.";
        }
        break;
    }
    case 2: {
        if (!created) {
            message = "Create linked list first.";
            break;
        }
        int value;
        if (readIntFromUser(value)) {
            list.Prepend(value);
            message = "Value prepended.";
        } else {
            message = "Invalid integer value.";
        }
        break;
    }
    case 3: {
        if (!created) {
            message = "Create linked list first.";
            break;
        }
        int index, value;
        mvprintw(15, 2, "Enter index: ");
        echo();
        curs_set(1);
        char input[32];
        move(15, 15);
        getnstr(input, 31);
        index = atoi(input);
        mvprintw(16, 2, "Enter value: ");
        move(16, 15);
        getnstr(input, 31);
        value = atoi(input);
        noecho();
        curs_set(0);
        try {
            list.InsertAt(value, index);
            message = "Value inserted.";
        } catch (...) {
            message = "Invalid index!";
        }
        break;
    }
    case 4:
        if (!created) {
            message = "Create linked list first.";
        } else {
            message = "List printed above.";
        }
        break;
    case 5:
        screen = Screen::MainMenu;
        break;
    default:
        break;
    }
}

void createSequence()
{
    if (intSequence) delete intSequence;
    if (bitSequence) delete bitSequence;
    
    if (currentSeqType == SequenceType::BitSequence) {
        bitSequence = new BitSequence();
        sequenceCreated = true;
    } else {
        if (currentSeqImpl == SequenceImplType::Mutable) {
            if (currentSeqType == SequenceType::ArraySequence) {
                intSequence = new MutableArraySequence<int>();
            } else {
                intSequence = new ListSequence<int>();
            }
        } else {
            if (currentSeqType == SequenceType::ArraySequence) {
                intSequence = new ImmutableArraySequence<int>();
            } else {
                intSequence = new ListSequence<int>();
            }
        }
        sequenceCreated = true;
    }
    sequenceMessage = "Sequence created.";
}

void handleSequenceTypeAction(int selected, Screen& screen)
{
    switch (selected) {
    case 0:
        currentSeqType = SequenceType::ArraySequence;
        break;
    case 1:
        currentSeqType = SequenceType::ListSequence;
        break;
    case 2:
        currentSeqType = SequenceType::BitSequence;
        break;
    }
    
    if (currentSeqType == SequenceType::BitSequence) {
        createSequence();
        screen = Screen::SequenceActions;
    } else {
        screen = Screen::SequenceImpl;
    }
}

void handleSequenceImplAction(int selected, Screen& screen)
{
    switch (selected) {
    case 0:
        currentSeqImpl = SequenceImplType::Mutable;
        break;
    case 1:
        currentSeqImpl = SequenceImplType::Immutable;
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
    if (!sequenceCreated && selected != 0 && selected != 5) {
        message = "Create sequence first!";
        return;
    }
    
    try {
        switch (selected) {
        case 0: // Create
            createSequence();
            message = "Sequence created.";
            break;
            
        case 1: // Append
            if (currentSeqType == SequenceType::BitSequence) {
                bool val;
                mvprintw(15, 2, "Enter value (0 or 1): ");
                echo();
                curs_set(1);
                char input[5];
                move(15, 25);
                getnstr(input, 4);
                noecho();
                curs_set(0);
                val = (input[0] == '1');
                bitSequence->Append(val);
                message = "Value appended.";
            } else {
                int val;
                if (readIntFromUser(val)) {
                    intSequence->Append(val);
                    message = "Value appended.";
                } else {
                    message = "Invalid value.";
                }
            }
            break;
            
        case 2: // Prepend
            if (currentSeqType == SequenceType::BitSequence) {
                bool val;
                mvprintw(15, 2, "Enter value (0 or 1): ");
                echo();
                curs_set(1);
                char input[5];
                move(15, 25);
                getnstr(input, 4);
                noecho();
                curs_set(0);
                val = (input[0] == '1');
                bitSequence->Prepend(val);
                message = "Value prepended.";
            } else {
                int val;
                if (readIntFromUser(val)) {
                    intSequence->Prepend(val);
                    message = "Value prepended.";
                } else {
                    message = "Invalid value.";
                }
            }
            break;
            
        case 3: // Insert At
            {
                int index;
                mvprintw(15, 2, "Enter index: ");
                char input[32];
                echo();
                curs_set(1);
                move(15, 15);
                getnstr(input, 31);
                index = atoi(input);
                if (currentSeqType == SequenceType::BitSequence) {
                    mvprintw(16, 2, "Enter value (0 or 1): ");
                    move(16, 25);
                    getnstr(input, 4);
                    bool val = (input[0] == '1');
                    noecho();
                    curs_set(0);
                    bitSequence->InsertAt(val, index);
                } else {
                    mvprintw(16, 2, "Enter value: ");
                    move(16, 15);
                    getnstr(input, 31);
                    int val = atoi(input);
                    noecho();
                    curs_set(0);
                    intSequence->InsertAt(val, index);
                }
                message = "Value inserted.";
            }
            break;
            
        case 4: // Print
            message = "Values printed above.";
            break;
            
        case 5: // Back
            screen = Screen::MainMenu;
            break;
        }
    } catch (std::out_of_range& e) {
        message = "Index out of range!";
        noecho();
        curs_set(0);
    } catch (std::invalid_argument& e) {
        message = e.what();
        noecho();
        curs_set(0);
    }
}

bool readIntFromUser(int& value)
{
    char input[32];
    mvprintw(15, 2, "Enter integer value: ");
    clrtoeol();
    echo();
    curs_set(1);
    move(15, 23);
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
    mvprintw(15, 2, "Enter value (0 or 1): ");
    clrtoeol();
    echo();
    curs_set(1);
    move(15, 25);
    getnstr(input, sizeof(input) - 1);
    noecho();
    curs_set(0);
    if (input[0] == '0') value = false;
    else if (input[0] == '1') value = true;
    else return false;
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
    mvprintw(1, 2, "Choose data structure");
    mvprintw(2, 2, "Use Left/Right or Tab, Enter/Space to select, q to quit.");
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

    Button mainButtons[MAIN_BUTTON_COUNT] = {
        {"Dynamic Array", 4, 2, 20},
        {"Linked List", 4, 26, 20},
        {"Sequence", 4, 50, 20},
    };

    Button dynamicArrayButtons[DYNAMIC_ARRAY_BUTTON_COUNT] = {
        {"Create", 4, 2, 16},
        {"Add", 4, 22, 16},
        {"Set", 4, 42, 16},
        {"Print", 4, 62, 16},
        {"Back", 4, 82, 16},
    };

    Button linkedListButtons[LINKED_LIST_BUTTON_COUNT] = {
        {"Create", 4, 2, 16},
        {"Append", 4, 22, 16},
        {"Prepend", 4, 42, 16},
        {"InsertAt", 4, 62, 16},
        {"Print", 4, 82, 16},
        {"Back", 4, 102, 16},
    };

    Button seqTypeButtons[SEQUENCE_TYPE_BUTTON_COUNT] = {
        {"ArraySequence", 4, 2, 20},
        {"ListSequence", 4, 26, 20},
        {"BitSequence", 4, 50, 20},
    };

    Button seqImplButtons[SEQUENCE_IMPL_BUTTON_COUNT] = {
        {"Mutable", 4, 2, 20},
        {"Immutable", 4, 26, 20},
        {"Back", 4, 50, 20},
    };

    Button seqActionButtons[SEQUENCE_ACTION_BUTTON_COUNT] = {
        {"Create", 4, 2, 16},
        {"Append", 4, 22, 16},
        {"Prepend", 4, 42, 16},
        {"InsertAt", 4, 62, 16},
        {"Print", 4, 82, 16},
        {"Back", 4, 102, 16},
    };

    while (screen != Screen::Exit) {
        if (screen == Screen::MainMenu) {
            drawMainMenu(mainButtons, selected);
            int ch = getch();
            if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            } else if (ch == KEY_LEFT) {
                selected = (selected + MAIN_BUTTON_COUNT - 1) % MAIN_BUTTON_COUNT;
            } else if (ch == KEY_RIGHT || ch == '\t') {
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
            drawDynamicArrayScreen(dynamicArrayButtons, dynamicArraySelected, dynamicArray, dynamicArrayCreated, dynamicArrayMessage);
            int ch = getch();
            if (ch == KEY_LEFT) {
                dynamicArraySelected = (dynamicArraySelected + DYNAMIC_ARRAY_BUTTON_COUNT - 1) % DYNAMIC_ARRAY_BUTTON_COUNT;
            } else if (ch == KEY_RIGHT || ch == '\t') {
                dynamicArraySelected = (dynamicArraySelected + 1) % DYNAMIC_ARRAY_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleDynamicArrayAction(dynamicArraySelected, screen, dynamicArray, dynamicArrayCreated, dynamicArrayMessage);
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::MainMenu;
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
        else if (screen == Screen::LinkedList) {
            drawLinkedListScreen(linkedListButtons, linkedListSelected, linkedList, linkedListCreated, linkedListMessage);
            int ch = getch();
            if (ch == KEY_LEFT) {
                linkedListSelected = (linkedListSelected + LINKED_LIST_BUTTON_COUNT - 1) % LINKED_LIST_BUTTON_COUNT;
            } else if (ch == KEY_RIGHT || ch == '\t') {
                linkedListSelected = (linkedListSelected + 1) % LINKED_LIST_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleLinkedListAction(linkedListSelected, screen, linkedList, linkedListCreated, linkedListMessage);
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::MainMenu;
            } else if (ch == 'q' || ch == 'Q') {
                screen = Screen::Exit;
            }
        }
        else if (screen == Screen::SequenceType) {
            drawSequenceTypeScreen(seqTypeButtons, seqTypeSelected);
            int ch = getch();
            if (ch == KEY_LEFT) {
                seqTypeSelected = (seqTypeSelected + SEQUENCE_TYPE_BUTTON_COUNT - 1) % SEQUENCE_TYPE_BUTTON_COUNT;
            } else if (ch == KEY_RIGHT || ch == '\t') {
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
        else if (screen == Screen::SequenceImpl) {
            drawSequenceImplScreen(seqImplButtons, seqImplSelected);
            int ch = getch();
            if (ch == KEY_LEFT) {
                seqImplSelected = (seqImplSelected + SEQUENCE_IMPL_BUTTON_COUNT - 1) % SEQUENCE_IMPL_BUTTON_COUNT;
            } else if (ch == KEY_RIGHT || ch == '\t') {
                seqImplSelected = (seqImplSelected + 1) % SEQUENCE_IMPL_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleSequenceImplAction(seqImplSelected, screen);
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
            if (ch == KEY_LEFT) {
                seqActionSelected = (seqActionSelected + SEQUENCE_ACTION_BUTTON_COUNT - 1) % SEQUENCE_ACTION_BUTTON_COUNT;
            } else if (ch == KEY_RIGHT || ch == '\t') {
                seqActionSelected = (seqActionSelected + 1) % SEQUENCE_ACTION_BUTTON_COUNT;
            } else if (ch == '\n' || ch == '\r' || ch == ' ') {
                handleSequenceAction(seqActionSelected, screen, sequenceMessage);
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                screen = Screen::SequenceType;
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