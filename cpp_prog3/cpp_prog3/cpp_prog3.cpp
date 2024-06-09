#include <iostream>

#include "functions.h"

using namespace std;


MenuSystem* MenuSystem::instance = nullptr;


int main(int argc, char* argv[])
{
    MenuSystem menuSystem;
    MenuSystem::instance = &menuSystem;

    menuSystem.Init("file.txt");
    menuSystem.Run(argc, argv);
    
    
    return 0;
}


/*
for (const auto& mainMenu : menuSystem.mainMenus) {
        cout << "Main Menu: " << mainMenu.name << endl;
        cout << "  Color: (" << mainMenu.color.r << ", " << mainMenu.color.g << ", " << mainMenu.color.b << ")" << endl;
        cout << "  Position: (" << mainMenu.position.x << ", " << mainMenu.position.y << ")" << endl;
        cout << "  Highlight Color: (" << mainMenu.highlightColor.r << ", " << mainMenu.highlightColor.g << ", " << mainMenu.highlightColor.b << ")" << endl;
        cout << "  Size: (" << mainMenu.size.width << ", " << mainMenu.size.height << ")" << endl;

        for (const auto& subMenu : mainMenu.submenus) {
            cout << "  Sub Menu: " << subMenu.name << endl;
            cout << "    Color: (" << subMenu.color.r << ", " << subMenu.color.g << ", " << subMenu.color.b << ")" << endl;
            cout << "    Position: (" << subMenu.position.x << ", " << subMenu.position.y << ")" << endl;
            cout << "    Highlight Color: (" << subMenu.highlightColor.r << ", " << subMenu.highlightColor.g << ", " << subMenu.highlightColor.b << ")" << endl;
            cout << "    Size: (" << subMenu.size.width << ", " << subMenu.size.height << ")" << endl;

            for (const auto& dialog : subMenu.dialogs) {
                cout << "    Dialog Box: " << dialog.name << endl;
                cout << "      Color: (" << dialog.color.r << ", " << dialog.color.g << ", " << dialog.color.b << ")" << endl;
                cout << "      Position: (" << dialog.position.x << ", " << dialog.position.y << ")" << endl;
                cout << "      Highlight Color: (" << dialog.highlightColor.r << ", " << dialog.highlightColor.g << ", " << dialog.highlightColor.b << ")" << endl;
                cout << "      Size: (" << dialog.size.width << ", " << dialog.size.height << ")" << endl;

                for (const auto& button : dialog.buttons) {
                    cout << "      Button: " << button.name << endl;
                    cout << "        Color: (" << button.color.r << ", " << button.color.g << ", " << button.color.b << ")" << endl;
                    cout << "        Position: (" << button.position.x << ", " << button.position.y << ")" << endl;
                    cout << "        Highlight Color: (" << button.highlightColor.r << ", " << button.highlightColor.g << ", " << button.highlightColor.b << ")" << endl;
                    cout << "        Size: (" << button.size.width << ", " << button.size.height << ")" << endl;
                }
            }
        }

        for (const auto& dialog : mainMenu.dialogs) {
            cout << "  Dialog Box: " << dialog.name << endl;
            cout << "    Color: (" << dialog.color.r << ", " << dialog.color.g << ", " << dialog.color.b << ")" << endl;
            cout << "    Position: (" << dialog.position.x << ", " << dialog.position.y << ")" << endl;
            cout << "    Highlight Color: (" << dialog.highlightColor.r << ", " << dialog.highlightColor.g << ", " << dialog.highlightColor.b << ")" << endl;
            cout << "    Size: (" << dialog.size.width << ", " << dialog.size.height << ")" << endl;

            for (const auto& button : dialog.buttons) {
                cout << "    Button: " << button.name << endl;
                cout << "      Color: (" << button.color.r << ", " << button.color.g << ", " << button.color.b << ")" << endl;
                cout << "      Position: (" << button.position.x << ", " << button.position.y << ")" << endl;
                cout << "      Highlight Color: (" << button.highlightColor.r << ", " << button.highlightColor.g << ", " << button.highlightColor.b << ")" << endl;
                cout << "      Size: (" << button.size.width << ", " << button.size.height << ")" << endl;
            }
        }
    }
*/