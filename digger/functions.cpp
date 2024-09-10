#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "functions.h"
#include <iostream>

void Cell::Draw(Color color) {
        glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
        glBegin(GL_QUADS);
        glVertex2f(AbsolutePosition.x, AbsolutePosition.y);
        glVertex2f(AbsolutePosition.x + CellSize, AbsolutePosition.y);
        glVertex2f(AbsolutePosition.x + CellSize, AbsolutePosition.y + CellSize);
        glVertex2f(AbsolutePosition.x, AbsolutePosition.y + CellSize);
        glEnd();
}

GLuint GameObject::loadTexture(const std::string& filename) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        int stride = width * nrChannels;
        for (int y = 0; y < height / 2; ++y) {
            for (int x = 0; x < stride; ++x) {
                std::swap(data[y * stride + x], data[(height - 1 - y) * stride + x]);
            }
        }

        std::cout << "Image loaded: " << width << "x" << height << ", " << nrChannels << " channels" << std::endl;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);

        textureWidth = width;
        textureHeight = height;
    }
    else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }

    return texture;
}

void GameObject::Draw(const string& file, bool NeedToLoad) {
    if (texture == 0 or NeedToLoad) {
        loadTexture(file);
    }

    glEnable(GL_TEXTURE_2D); // Enable 2D texturing

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Define the size of the image
    float imageWidth = CellSize; // Adjust these values as needed
    float imageHeight = CellSize; // Adjust these values as needed

    // Apply transformations
    glPushMatrix();
    glTranslatef(posX, posY, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Draw a textured quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-imageWidth / 2, -imageHeight / 2, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(imageWidth / 2, -imageHeight / 2, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(imageWidth / 2, imageHeight / 2, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-imageWidth / 2, imageHeight / 2, 0.0f);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D); // Disable 2D texturing
    glDisable(GL_BLEND); // Disable blending

}

void FireBall::Move() {
    if (direction == 0.0f) { 
        posX += CellSize * FireballSpeed;
        if (posX >= (FieldPos.x + 1) * CellSize) {
            FieldPos.x += 1;
        }
    }
    else if (direction == 90.0f) { 
        posY += CellSize * FireballSpeed;
        if (posY >= (FieldPos.y + 1) * CellSize) {
            FieldPos.y += 1;
        }
    }
    else if (direction == 180.0f) { 
        posX -= CellSize * FireballSpeed;
        if (posX <= (FieldPos.x - 1) * CellSize) {
            FieldPos.x -= 1;
        }
    }
    else if (direction == 270.0f) { 
        posY -= CellSize * FireballSpeed;
        if (posY <= (FieldPos.y - 1) * CellSize) {
            FieldPos.y -= 1;
        }
    }
}

void FireBall::Draw() {
    if (isActive) {
        glColor3f(1.0f, 0.0f, 0.0f); 
        glBegin(GL_QUADS);
        glVertex2f(posX, posY);
        glVertex2f(posX + CellSize, posY);
        glVertex2f(posX + CellSize, posY + CellSize);
        glVertex2f(posX, posY + CellSize);
        glEnd();
    }
}

GoldBag::GoldBag(const Cell& cell) :cell() {
    this->IsBroken = false;
    this->isFalling = false;
    this->NeedToLoadBroken = false;
    this->posX = cell.AbsolutePosition.x + CellSize / 2;  
    this->posY = cell.AbsolutePosition.y + CellSize / 2;
    this->FieldPos = cell.FieldPosition;  
}

Emerald::Emerald(const Cell& cell) :cell() {

    this->posX = cell.AbsolutePosition.x + CellSize / 2;  
    this->posY = cell.AbsolutePosition.y + CellSize / 2;
    this->FieldPos = cell.FieldPosition;  
}

Nobbin::Nobbin(float x, float y, int X, int Y) {
    curpos.x = X;
    curpos.y = Y;
    posX = x;
    posY = y;
}

void Nobbin::BFSPathfinding(Point target, GameLevel& level) 
{
    vector<Point> calculated_path;

    if (curpos == target) {
        path = calculated_path; 
    }

    queue<Point> toVisit;
    map<Point, Point> cameFrom;

    toVisit.push(curpos);
    cameFrom[curpos] = curpos;



    while (!toVisit.empty()) {
        Point current = toVisit.front();
        toVisit.pop();


        if (current == target) {

            while (current != curpos) {
                calculated_path.push_back(current);
                current = cameFrom[current];
            }

            reverse(calculated_path.begin(), calculated_path.end());

            
        }

        vector<pair<int, int>> coords = {
            {current.x + 1, current.y},
            {current.x - 1, current.y},
            {current.x, current.y + 1},
            {current.x, current.y - 1}
        };

        for (auto& coord : coords) {
            Point next = { coord.first, coord.second };

            bool isValid = false;
            for (auto& cell : level.Path) {
                if (cell.FieldPosition == next) {
                    isValid = true;
                    break;
                }
            }

            if (isValid && cameFrom.find(next) == cameFrom.end()) {
                toVisit.push(next);
                cameFrom[next] = current;
            }
        }

    }

    path = calculated_path;
}

void Player::PlayerDeath(Game& game) {
    lives -= 1;
    if (lives <= 0) {
        IsAlive = false;
        return;
    }
    glutTimerFunc(16, game.ResumeAfterDeathWrapper, 0);
}

void Player::ResumeAfterDeath(Game& game) {
    curpos = { 0, 0 };
    posX = 0 * CellSize + 23;  
    posY = 0 * CellSize + 23;

    cout << "Player respawned at (0, 0)." << endl;

    glutTimerFunc(1000, game.MoveNobbinWrapper, 0);  
    glutTimerFunc(1000, game.FallingGoldBagsWrapper, 0);  
    glutTimerFunc(16, game.TimerWrapper, 0);  

    glutPostRedisplay();
}

void Player::Shoot() {
    FireBall newFireball(curpos, angle);
    fireballs.push_back(newFireball);
}

void Player::UpdateFireballs(vector<Cell>& earth, vector<Nobbin>& nobbins) {
    for (auto& fireball : fireballs) {
        if (fireball.isActive) {
            fireball.Move();

            for (auto it = nobbins.begin(); it != nobbins.end(); ) {
                if (it->curpos == fireball.FieldPos) {
                    fireball.isActive = false;  
                    it = nobbins.erase(it);     
                    cout << "Nobbin hit by fireball and removed!" << endl;
                }
                else {
                    ++it;
                }
            }

            for (auto& cell : earth) {
                if (cell.FieldPosition == fireball.FieldPos && cell.IsLand) {
                    fireball.isActive = false;  
                }
            }
        }
    }
}

void Player::DrawFireballs() {
    for (auto& fireball : fireballs) {
        fireball.Draw();
    }
}

void GameLevel::DrawLevel() {

    for (auto& cell : Earth) {
        cell.Draw(earthColor);
    }

    for (auto& cell : Path) {
        cell.Draw(pathColor);
    }

    for (auto& goldBag : GoldBags) {
        goldBag.cell.Draw({ 255, 215, 0 });
    }
}

void GameLevel::InitializeLevel() {
    Earth.clear();
    Path.clear();
    Emeralds.clear();
    GoldBags.clear();

    for (int y = 0; y < FieldSizeY; ++y) {
        for (int x = 0; x < FieldSizeX; ++x) {
            Cell cell{ {x * CellSize, y * CellSize },{ x, y }, false, false, false, false, false, false };

            if ((x == 0 || x == 9) && y >= 0 && y <= 7) {
                cell.IsPath = true;
                Path.push_back(cell);
            }
            else if ((x == 6 && y >= 3 && y <= 6) || (x == 8 && y >= 3 && y <= 6)) {
                Emerald cur = Emerald(cell);
                cur.cell.AbsolutePosition.x = cell.AbsolutePosition.x;
                cur.cell.AbsolutePosition.y = cell.AbsolutePosition.y;
                cur.cell.FieldPosition.x = cell.FieldPosition.x;
                cur.cell.FieldPosition.y = cell.FieldPosition.y;
                cur.cell.IsEmerald = true;
                Emeralds.push_back(cur);
            }
            else if ((x == 7 && y == 2) || (x == 7 && y == 7)) {
                GoldBag cur = GoldBag(cell);
                cur.cell.AbsolutePosition.x = cell.AbsolutePosition.x;
                cur.cell.AbsolutePosition.y = cell.AbsolutePosition.y;
                cur.cell.FieldPosition.x = cell.FieldPosition.x;
                cur.cell.FieldPosition.y = cell.FieldPosition.y;
                cur.cell.IsGoldBag = true;
                GoldBags.push_back(cur);
            }
            else {
                cell.IsLand = true;
                Earth.push_back(cell);
            }
        }
    }
    cout << "Level initialized.\n";
}

void Game::CheckConditions() {

    if (player.lives == 0) {
        cout << "end";
    }

    for (auto iter = level.Earth.rbegin(); iter != level.Earth.rend(); ) {
        if (iter->FieldPosition == player.curpos) {
            iter->IsLand = false;
            iter->IsPath = true;
            level.Path.push_back(*iter);
            iter = vector<Cell>::reverse_iterator(level.Earth.erase((++iter).base()));
        }
        else {
            ++iter;
        }
    }

    for (auto it = level.Emeralds.begin(); it != level.Emeralds.end(); ) {


        if (it->cell.FieldPosition == player.curpos) {
            it->cell.IsEmerald = false;
            player.Emeralds += 1;
            it->cell.IsPath = true;
            level.Path.push_back(it->cell);
            it = level.Emeralds.erase(it);
        }
        else {
            ++it;  
        }
    }

    for (auto it = level.GoldBags.begin(); it != level.GoldBags.end(); ) {
        for (auto nb = level.Nobbins.begin(); nb != level.Nobbins.end(); ) {
            if ((nb->FieldPos == it->FieldPos) and (it->isFalling)) {
                nb->die();
            }
            ++nb;
        }
        if ((player.curpos == it->FieldPos) and (it->isFalling)) {
            player.PlayerDeath(*instance);

        }
        if (!it->IsBroken) {

            if ((it->FieldPos.x - 1 == player.curpos.x) && it->FieldPos.y == player.curpos.y) {
                it->RightDirection = true;
                it->LeftDirection = false;

            }

            if ((it->FieldPos.x + 1 == player.curpos.x) && it->FieldPos.y == player.curpos.y) {
                it->RightDirection = false;
                it->LeftDirection = true;
            }

            if (it->FieldPos == player.curpos) {
                level.Path.push_back(it->cell);
                int dif = it->RightDirection ? 1 : -1;
                it->cell.FieldPosition.x += dif;
                it->cell.AbsolutePosition.x += (dif * CellSize);
                it->FieldPos.x += dif;
                it->posX += dif * CellSize;
                auto& lastCell = level.Path.back(); 
                lastCell.IsPath = true;
                lastCell.IsGoldBag = false;

            }

            for (auto pth = level.Path.begin(); pth != level.Path.end(); ) {
                if (pth->FieldPosition.x == it->FieldPos.x and pth->FieldPosition.y == it->FieldPos.y - 1) {
                    it->isFalling = true;
                }
                ++pth;
            }
        }
        else {
            if (it->cell.FieldPosition == player.curpos) {
                it->cell.IsGoldBag = false;
                player.goldbags += 1;
                it->cell.IsPath = true;
                level.Path.push_back(it->cell);
                it = level.GoldBags.erase(it);
                continue;
            }

        }
        cout << it->isFalling << endl;
        ++it;

    }

    for (auto it = level.Nobbins.begin(); it != level.Nobbins.end(); ) {
        cout << it->curpos.x << " " << it->curpos.y << "nob"<<endl;
        cout << player.curpos.x << " " << player.curpos.y << "pl" << endl;
        if (it->curpos == player.curpos) player.PlayerDeath(*instance);
        ++it;
    }
    glutPostRedisplay();
}

void Game::handleKey(unsigned char key, int x, int y) {

    unsigned int currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (currentTime - lastMoveTime >= moveDelay) {
        switch (key) {
        case 'w':
            if (player.curpos.y != FieldSizeY - 1) {
                player.posY += CellSize;
                player.curpos.y += 1;
                player.angle = 90.0f;  
            }
            CheckConditions();
            break;
        case 's':
            if (player.curpos.y != 0) {
                player.posY -= CellSize;
                player.curpos.y -= 1;
                player.angle = 270.0f;
            }
            CheckConditions();
            break;
        case 'a':
            if (player.curpos.x != 0) {
                player.posX -= CellSize;
                player.curpos.x -= 1;
                player.angle = 180.0f; 
            }
            CheckConditions();
            break;
        case 'd':


            if (player.curpos.x != FieldSizeX - 1) {
                player.posX += CellSize;
                player.curpos.x += 1;
                player.angle = 0.0f;  
            }
            CheckConditions();
            break;
        case 27:
            exit(0);
            break;
        case 'q':
            cout << "shoot";
            if (player.fireball_numb > 0) {
                player.Shoot();
                player.fireball_numb -= 1;
            }
            break;
        }
        lastMoveTime = currentTime;
    }
}

bool Game::IsPositionOccupiedByNobbin(Point position) {
    for (const Nobbin& nob : enemies) {
        if (nob.curpos == position) {
            return true;  
        }
    }
    return false;
}

void Game::ChangePositionForNobbin() {
    for (class Nobbin& nob : enemies) {
        nob.BFSPathfinding(player.curpos, level);

        // Проверяем, что путь не пустой
        if (!nob.path.empty()) {
            Point nextStep = nob.path.front();

            // Проверяем, что следующая клетка не занята другим Nobbin
            if (!IsPositionOccupiedByNobbin(nextStep)) {
                nob.path.erase(nob.path.begin());  // Удаляем первый шаг
                nob.curpos = nextStep;
                nob.posX = nextStep.x * CellSize + 23;
                nob.posY = nextStep.y * CellSize + 23;

                cout << "Nobbin moved to: (" << nextStep.x << ", " << nextStep.y << ")" << endl;
            }
            else {
                cout << "Nobbin cannot move to: (" << nextStep.x << ", " << nextStep.y << "), position is occupied!" << endl;
            }
        }
        else if (nob.curpos == player.curpos) {
            player.PlayerDeath(*instance);
            cout << "Nobbin is on the same cell as the player!" << endl;
        }
    }
}

void Game::MoveNobbin(int value) {
    ChangePositionForNobbin();
    // Redisplay after timer
    glutPostRedisplay();
    // Set timer again for continuous updates
    glutTimerFunc(1000, MoveNobbinWrapper, 0);
}

void Game::FallingGoldBags(int value) {
    for (auto it = level.GoldBags.begin(); it != level.GoldBags.end(); ) {

        if (it->isFalling) {
            for (auto iter = level.Path.rbegin(); iter != level.Path.rend(); ) {
                if ((it->FieldPos.y - 1 == iter->FieldPosition.y) and (it->FieldPos.x == iter->FieldPosition.x)) {
                    it->cell.FieldPosition.y -= 1;
                    it->cell.AbsolutePosition.y -= CellSize;
                    it->FieldPos.y -= 1;
                    it->posY -= CellSize;
                    it->isFalling = true;

                    break;

                }
                else it->isFalling = false;
                ++iter;
            }

            if (!it->isFalling) {
                it->IsBroken = true;
                it->NeedToLoadBroken = true;
            }
        }

        ++it;
    }

    glutPostRedisplay();
    if (size(level.GoldBags) != 0) {
        glutTimerFunc(300, FallingGoldBagsWrapper, 0);
    }
}

void Game::CreateNobbin(int index) {
    if ((index < 3) and (NobbinsCreated == false)) {  // Создаем только трех ноббинов
        Nobbin enemy(0, 0, 0, 0);
        enemies.push_back(enemy);


        // Устанавливаем таймер для следующего ноббина
        glutTimerFunc(750, CreateNobbinWrapper, index + 1);
    }
    if (index == 3) {
        NobbinsCreated = true;
    }

}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw the level first
    level.DrawLevel();

    // Draw the player on top of the level

    for (auto& goldBag : level.GoldBags) {
        if (goldBag.IsBroken) {
            goldBag.Draw("gold.png", goldBag.NeedToLoadBroken);
            goldBag.NeedToLoadBroken = false;
        }
        else goldBag.Draw("goldbag.png");
    }

    player.Draw("digger.png");
    if (size(enemies) != 0) {
        for (class Nobbin& nob : enemies) {
            nob.Draw("NOB.png");
        }
    }

    for (class Emerald& emerald : level.Emeralds) {
        emerald.Draw();
    }
    player.DrawFireballs();

    glutSwapBuffers();
}

void Game::timer(int value) {
    // Update fireballs
    player.UpdateFireballs(level.Earth, enemies);

    // Redisplay after timer
    glutPostRedisplay();
    // Set timer again for continuous updates
    glutTimerFunc(16, TimerWrapper, 0);
}

void Game::setupCoordinateSystem() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, FieldSizeX * CellSize, 0, FieldSizeY * CellSize);
    glMatrixMode(GL_MODELVIEW);
}

void Game::Run(int argc, char* argv[]) {
    instance = this;
    this->NobbinsCreated = false;
    glutInit(&argc, argv);
    player.curpos = { 0,0 };
    player.fireball_numb = 3;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(FieldSizeX * CellSize, FieldSizeY * CellSize);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");
    glClearColor(0.5, 1.0, 1.0, 1.0);
    setupCoordinateSystem();
    level.InitializeLevel();

    glutDisplayFunc(DisplayWrapper);
    glutKeyboardFunc(KeyWrapper);
    glutTimerFunc(16, TimerWrapper, 0); // Start the timer
    glutTimerFunc(10000, CreateNobbinWrapper, 0);
    glutTimerFunc(10000, MoveNobbinWrapper, 0);
    glutTimerFunc(1000, FallingGoldBagsWrapper, 0);
    glutMainLoop();
}