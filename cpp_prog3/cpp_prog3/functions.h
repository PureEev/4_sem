#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <fstream>
#include <gl/glut.h>


using namespace std;

struct Point {
    int x, y;
};

struct Color {
    int r, g, b;
};

struct Size {
    int height, width;
};

class Action {
public:
    virtual void execute() = 0;
    virtual ~Action() = default; // Добавление виртуального деструктора
};

class MenuElement {
public:
    string name;
    Color color;
    Point position;
    Color highlightColor;
    Size size;
    bool Selected;

    MenuElement(string name, Color color, Point position, Color highlightColor, Size size)
        : name(name), color(color), position(position), highlightColor(highlightColor), size(size) {}

    virtual ~MenuElement() = default;

    void TakeDataForElement(string line) {

        regex name_regex(R"(Name = \"([^\"]*)\")");
        regex color_regex(R"(Color = \((\d+),(\d+),(\d+)\))");
        regex highlightcolor_regex(R"(Higlightcolor = \((\d+),(\d+),(\d+)\))");
        regex position_regex(R"(Position = \((\d+),(\d+)\))");
        regex size_regex(R"(Size = \((\d+),(\d+)\))");
        smatch matches;

        if (regex_search(line, matches, name_regex)) {
            name = matches.str(1);
        }
        if (regex_search(line, matches, color_regex)) {
            color.r = stoi(matches[1]) / 255.0;
            color.g = stoi(matches[2]) / 255.0;
            color.b = stoi(matches[3]) / 255.0;
        }
        if (regex_search(line, matches, highlightcolor_regex)) {
            highlightColor.r = stoi(matches[1]) / 255.0;
            highlightColor.g = stoi(matches[2]) / 255.0;
            highlightColor.b = stoi(matches[3]) / 255.0;
        }
        if (regex_search(line, matches, position_regex)) {
            position.x = stoi(matches[1]);
            position.y = stoi(matches[2]);
        }
        if (regex_search(line, matches, size_regex)) {
            size.width = stoi(matches[1]);
            size.height = stoi(matches[2]);

        }
        
    }

    void RenderText(const std::string& text, float x, float y) {
        glColor3f(0, 0, 0); // Установить цвет текста
        glRasterPos2f(x, y); // Установить начальную позицию для текста
        for (const char& c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Отобразить символ
        }
    }

    void Draw() {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        glVertex2f(position.x, position.y);
        glVertex2f(position.x + size.width, position.y);
        glVertex2f(position.x + size.width, position.y + size.height);
        glVertex2f(position.x, position.y + size.height);
        glEnd();

        if (Selected) {
            glColor3f(highlightColor.r, highlightColor.g, highlightColor.b);
            glLineWidth(8.0); // Установка толщины линии в 2.0
            glBegin(GL_LINE_LOOP);
            glVertex2f(position.x, position.y);
            glVertex2f(position.x + size.width, position.y);
            glVertex2f(position.x + size.width, position.y + size.height);
            glVertex2f(position.x, position.y + size.height);
            glEnd();


        }
        RenderText(name, position.x + 0.25 * size.width, position.y + 0.25 * size.height);

    }
    

   
};

class Button : public MenuElement {
public:
    Action* action; // Указатель на действие

    Button(string name, Color color, Point position, Color highlightColor, Size size, Action* action = nullptr)
        : MenuElement(name, color, position, highlightColor, size), action(action) {}

    ~Button() {
        delete action; // Освобождение памяти для действия
    }
};

class MainMenu : public MenuElement {
public:
    vector<class SubMenu> submenus;
    vector<class DialogBox> dialogs;
    vector<Action*> actions;

    MainMenu(string name, Color color, Point position, Color highlightColor, Size size)
        : MenuElement(name, color, position, highlightColor, size) {}

    ~MainMenu() {
        for (auto action : actions) {
            delete action; // Удаляем действия, чтобы избежать утечек памяти
        }
    }

    void AddSubMenuElement(const SubMenu& CurElement) {
        submenus.push_back(CurElement);
    }

    void AddDialogBoxElement(const DialogBox& CurElement) {
        dialogs.push_back(CurElement);
    }

    void AddActionElement(Action* CurElement) {
        actions.push_back(CurElement);
    }

   
};

class SubMenu : public MenuElement {
public:
    vector< class DialogBox> dialogs;
    vector<Action*> actions;

    SubMenu(string name, Color color, Point position, Color highlightColor, Size size)
        : MenuElement(name, color, position, highlightColor, size) {}

    ~SubMenu() {
        for (auto action : actions) {
            delete action; // Удаляем действия, чтобы избежать утечек памяти
        }
    }

    void AddDialogBoxElement(const DialogBox& CurElement) {
        dialogs.push_back(CurElement);
    }

    void AddActionElement(Action* CurElement) {
        actions.push_back(CurElement);
    }
};

class DialogBox : public MenuElement {
public:
    vector<Button> buttons;

    DialogBox(string name, Color color, Point position, Color highlightColor, Size size)
        : MenuElement(name, color, position, highlightColor, size) {}

    void addButton(const Button& button) {
        buttons.push_back(button);
    }
};

// Пример действий


class MenuSystem {
public:
    vector<class MainMenu> mainMenus;
    MainMenu* Selected = nullptr;
    static MenuSystem* instance;
    bool IsMainMenu;
    bool IsSubMenu;
    bool IsDialogBox;
    void Init(string filename) {

        ifstream file(filename);
        map<string, bool> DictionaryOfElements;
        DictionaryOfElements["IsMainMenu"] = false;
        DictionaryOfElements["IsSubMenu"] = false;
        DictionaryOfElements["IsDialogBox"] = false;
        DictionaryOfElements["ISbutton"] = false;
        string line;
        string CurElement;
        MainMenu* MainMenuCurrent = nullptr;
        SubMenu* SubMenuCurrent = nullptr;
        DialogBox* DialogBoxCurrent = nullptr;
        Button* ButtonCurrent = nullptr;
        smatch matches;

        while (getline(file, line)) {

            if (regex_search(line, matches, regex(R"(^\s*MainMenuBegin)"))) {
                DictionaryOfElements["IsMainMenu"] = true;
                MainMenuCurrent = new MainMenu("", { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0 });
                mainMenus.push_back(*MainMenuCurrent);
                MainMenuCurrent = &mainMenus.back();
            }
            else if (DictionaryOfElements["IsMainMenu"]) {
                MainMenuCurrent->TakeDataForElement(line);
                if (regex_search(line, matches, regex(R"(^\s*Size = \(\d+,\d+\)$)"))) {
                    DictionaryOfElements["IsMainMenu"] = false;
                }
            }
            else if (regex_search(line, matches, regex(R"(^\s*MainMenuEnd)"))) MainMenuCurrent = nullptr;
            else if (regex_search(line, matches, regex(R"(^\s*SubMenuBegin)"))) {
                SubMenuCurrent = new SubMenu("", { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0 });
                MainMenuCurrent->AddSubMenuElement(*SubMenuCurrent);
                SubMenuCurrent = &MainMenuCurrent->submenus.back();
                DictionaryOfElements["IsSubMenu"] = true;

            }
            else if (DictionaryOfElements["IsSubMenu"]) {
                SubMenuCurrent->TakeDataForElement(line);
                if (regex_search(line, matches, regex(R"(^\s*Size = \(\d+,\d+\)$)"))) {
                    DictionaryOfElements["IsSubMenu"] = false;
                }
            }
            else if (regex_search(line, matches, regex(R"(^\s*SubMenuEnd)"))) SubMenuCurrent = nullptr;
            else if (regex_search(line, matches, regex(R"(^\s*DialogBoxBegin)"))) {
                DictionaryOfElements["IsDialogBox"] = true;
                DialogBoxCurrent = new DialogBox("", { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0 });
                if (SubMenuCurrent) {
                    SubMenuCurrent->AddDialogBoxElement(*DialogBoxCurrent);
                    DialogBoxCurrent = &SubMenuCurrent->dialogs.back();
                }
                else if (MainMenuCurrent) {
                    MainMenuCurrent->AddDialogBoxElement(*DialogBoxCurrent);
                    DialogBoxCurrent = &MainMenuCurrent->dialogs.back();
                }
            }
            else if (DictionaryOfElements["IsDialogBox"]) {
                DialogBoxCurrent->TakeDataForElement(line);
                if (regex_search(line, matches, regex(R"(^\s*Size = \(\d+,\d+\)$)"))) {
                    DictionaryOfElements["IsDialogBox"] = false;
                }
            }
            else if (regex_search(line, matches, regex(R"(^\s*DialogBoxEnd)"))) DialogBoxCurrent = nullptr;
            else if (regex_search(line, matches, regex(R"(^\s*ButtonBegin)"))) {
                DictionaryOfElements["IsButton"] = true;
                ButtonCurrent = new Button("", { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }, { 0, 0 });
                DialogBoxCurrent->addButton(*ButtonCurrent);
                ButtonCurrent = &DialogBoxCurrent->buttons.back();
            }
            else if (DictionaryOfElements["IsButton"]) {
                ButtonCurrent->TakeDataForElement(line);
                if (regex_search(line, matches, regex(R"(^\s*Size = \(\d+,\d+\)$)"))) {
                    DictionaryOfElements["IsButton"] = false;
                }
            }
            else if (regex_search(line, matches, regex(R"(^\s*ButtonEnd)"))) ButtonCurrent = nullptr;

        }
        for (MainMenu& mainMenu : mainMenus) {
            if(mainMenu.submenus.size()!=0) mainMenu.submenus[0].Selected = true;
        }        
        mainMenus[0].Selected = true;
        IsMainMenu = true;
        IsSubMenu = false;
    }

    void addMainMenu(const MainMenu& menu) {
        mainMenus.push_back(menu);
    }

    static void DisplayWrapper() {
        instance->display();
    }

    void MoveNextMainMenu(bool forward) {

        if (mainMenus.empty()) return;

        size_t index = 0;
        bool movingForward = true;
        for (size_t i = 0; i < mainMenus.size(); ++i){

            if (mainMenus[i].Selected) {
                if (forward) {
                    if ((i == mainMenus.size()) or (i+1 == mainMenus.size())) return; //проверить
                    mainMenus[i].Selected = false;
                    mainMenus[i+1].Selected = true;
                    return;
                }
                else {
                    if ((i==0) or (i+1==0)) return;//проверить
                    mainMenus[i].Selected = false;
                    mainMenus[i - 1].Selected = true;
                }
            }
        }
        
    }
    void MoveNextSubMenu(bool forward) {

        size_t index = 0;
        bool movingForward = true;
        for (size_t i = 0; i < mainMenus.size(); ++i) {
            if (mainMenus[i].Selected) {
                for (size_t j = 0; j < mainMenus[i].submenus.size(); ++j) {
                    if (mainMenus[i].submenus[j].Selected) {
                        if (forward) {
                            if ((j == mainMenus[i].submenus.size()) or (j + 1 == mainMenus[i].submenus.size())) return; //проверить
                            mainMenus[i].submenus[j].Selected = false;
                            mainMenus[i].submenus[j + 1].Selected = true;
                            return;
                        }
                        else {
                            if (j == 0)  return;//проверить
                            mainMenus[i].submenus[j].Selected = false;
                            mainMenus[i].submenus[j - 1].Selected = true;
                        }
                    }
                }
            }
        }

    }
    
    /*void MoveButton() {
        if (IsMainMenu) {
            for (MainMenu& menu : mainMenus) {
                if (menu.Selected) {

                }
            }
        }
        if(IsSubMenu)
    }*/

    static void KeyWrapper(unsigned char key, int x, int y) {
        instance->handleKey(key, x, y);
    }


    void handleKey(int key, int x, int y) {
        bool IsPressed = false;
        switch (key) {
        case 13: // Enter key
            for (MainMenu& menus : mainMenus) {
                if (menus.Selected and menus.submenus.size() != 0 and !IsSubMenu) {
                    IsPressed = true;
                    IsMainMenu = false;
                    IsSubMenu = true;
                    break;
                }
                else if (IsSubMenu) {
                    for (SubMenu& submenu : menus.submenus) {
                        if (submenu.Selected and submenu.dialogs.size() != 0) {
                            IsPressed = true;
                            IsDialogBox = true;
                        }
                    }
                }
            }
            break;
        case 'w': // 'w' key
            if (IsMainMenu) {
                MoveNextMainMenu(false);
            }
            else if (IsSubMenu) {
                MoveNextSubMenu(false);
            }
            IsPressed = true;
            break;
        case 's': // 's' key
            if (IsMainMenu) {
                MoveNextMainMenu(true);
            }
            else if (IsSubMenu) {
                MoveNextSubMenu(true);
            }
            IsPressed = true;
            break;
        case 'a': // 'w' key
            //if (IsDialogBox) {
              //  MoveButton(false);
            //}
            IsPressed = true;
            break;
        case 'd': // 's' key
            //if (IsDialogBox) {
              //  MoveButton(true);
            //}
            IsPressed = true;
            break;
        case 27:
            if (IsSubMenu and !IsMainMenu) {
                IsMainMenu = true;
                IsSubMenu = false;
            }
            IsPressed = true;
            break;
        }
        
        if (IsPressed) {
            glutPostRedisplay();
            IsPressed = false;
        }
    }


    void display() {

        glClear(GL_COLOR_BUFFER_BIT); // Очистите экран

        if (IsMainMenu) {
            for (MainMenu& mainMenu : mainMenus) {
                mainMenu.Draw();
                if (IsDialogBox and mainMenu.Selected) {
                    mainMenu.dialogs[0].Draw();
                    for (Button& buttons : mainMenu.dialogs[0].buttons) {
                        buttons.Draw();
                    }
                }
            }
        }
        else if(IsSubMenu) {
            for (MainMenu& mainMenu : mainMenus) {
                if (mainMenu.Selected) {
                    for (SubMenu& submenu : mainMenu.submenus) {
                        submenu.Draw();
                        if (IsDialogBox and submenu.Selected) {
                            submenu.dialogs[0].Draw();
                            for (Button& buttons : submenu.dialogs[0].buttons) {
                                buttons.Draw();
                            }
                        }
                    }
                }
            }
        }
        
        glFlush(); // Выполните команды OpenGL и убедитесь, что они отрисованы
    }

    void Run(int argc, char* argv[]) {
        glutInit(&argc, argv); // Инициализировать GLUT
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Установить режим отображения
        glutInitWindowSize(1000, 1000); // Установить размер окна
        glutInitWindowPosition(0, 0); // Установить позицию окна
        glutCreateWindow("Система меню"); // Создать окно с заголовком "Система меню"
        glClearColor(0.5, 1.0, 1.0, 0.0); // Установить цвет фона
        gluOrtho2D(0.0, 1000.0, 0.0, 1000.0); // Установить систему координат
        glutDisplayFunc(DisplayWrapper); // Установить функцию отображения
        glutKeyboardFunc(KeyWrapper); // Установить функцию обработки клавиш
        glutMainLoop(); // Запустить основной цикл GLUT
    }

};























class ShowHelpDialog : public Action {
public:
    void execute() override {
        cout << "Showing help dialog..." << endl;
    }
};

class ExitApplication : public Action {
public:
    void execute() override {
        cout << "Exiting application..." << endl;
        exit(0); // Завершение программы
    }
};