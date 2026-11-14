# include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include<optional>
using namespace std;
using namespace sf;
class accounts {
 public:
    string id;
    string password;
    int balance;
};
void saveaccount(const accounts& a)
{
    ofstream file(a.id + ".txt");
    if (file.is_open())
    {
        file << a.id << endl;
        file << a.password << endl;
        file << a.balance << endl;
        file.close();
    }
    else
    {
        cout << "Error opening the file." << endl;
    }
}
bool loadaccount(const string& id, accounts& a)
{
    ifstream file(a.id + ".txt");
    if (file.is_open())
    {
        getline(file, a.id);
        getline(file, a.password);
        file >> a.balance;
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
int main()
{
    RenderWindow windows(VideoMode({ 1000, 600 }), "front page!");
    Texture pic;
    pic.loadFromFile("bg.png");
    Sprite bg(pic);
    bg.setScale({ 1000.f / pic.getSize().x, 600.f / pic.getSize().y });
    Font font;
    font.openFromFile("arial.ttf");
    accounts test;
    if (!loadaccount("user1", test))
    {
        saveaccount({ "user1", "1234", 1000000 });
    }
    if (!loadaccount("user2", test))
    {
        saveaccount({ "user2", "5678", 500000 });
    }
    if (!loadaccount("user3", test))
    {
        saveaccount({ "user3", "0000", 156000 });
    }
    if (!loadaccount("admin", test))
    {
        saveaccount({ "admin", "abcd",  0});
    }
    Text welcome(font, "WELCOME TO OUR BANK", 50);
    welcome.setPosition({ 30.f , 70.f });
    welcome.setFillColor({ Color::White });
    RectangleShape logint({ 320.f, 360.f });
    logint.setPosition({ 110.f, 150.f });
    logint.setFillColor(Color(10, 20, 40, 220));
    int portal = 0;
    Text logg(font, "LOGIN", 40);
    logg.setPosition({ 200, 160 });
    logg.setFillColor({ Color::White });
    RectangleShape userb({ 140, 30 });
    userb.setPosition({ 120, 220 });
    userb.setFillColor(Color::Blue);
    RectangleShape adminb({ 140, 30 });
    adminb.setPosition({ 280,220 });
    adminb.setFillColor(Color::Blue);
    Text userid(font, "USER", 20);
    userid.setPosition({ 150.f , 220.f });
    userid.setFillColor({ Color::White });
    Text adminid(font, "ADMIN", 20);
    adminid.setPosition({300.f , 220.f });
    adminid.setFillColor({ Color::White });
    Text login(font, "Login ID:", 20);
    login.setPosition({ 120.f , 280.f });
    int info=0;
    string enterid, enterpassword;
    login.setFillColor({ Color::White });
    RectangleShape id({ 270.f, 35.f });
    id.setPosition({ 120.f,310.f });
    id.setFillColor({ Color(20, 130, 125) });
    Text pass(font, "Password:", 20);
    pass.setPosition({ 120.f , 360.f });
    pass.setFillColor({ Color::White });
    RectangleShape password({ 270.f, 35.f });
    password.setPosition({ 120.f, 390.f });
    password.setFillColor({ Color(20, 130, 125) });
    RectangleShape logbox({140, 30 });
    logbox.setPosition({ 190, 440 });
    logbox.setFillColor(Color::Blue);
    Text log(font, "LOGIN", 25);
    log.setPosition({ 220, 440 });
    log.setFillColor({ Color::White });
    Text other(font, "Do not have an account signup?", 20);
    other.setPosition({ 130, 480 });
    other.setFillColor(Color:: White);
    Text idtext(font, "", 25);
    idtext.setPosition({ 120, 310 });
    idtext.setFillColor({ Color::Black });
    Text passtext(font, "", 25);
    passtext.setPosition({ 120, 390 });
    passtext.setFillColor({ Color::Black });
    Text error(font, "Wrong ID or password", 20);
    error.setPosition({ 150, 520 });
    error.setFillColor(Color::White);
    accounts current;
    string amount;
    bool showerror = false;
    while (windows.isOpen())
    {
        while (const optional event = windows.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                windows.close();
            }
            if (const auto* click = event-> getIf <Event::MouseButtonPressed>())
            {
                Vector2f mouse(click->position);
                if (userb.getGlobalBounds().contains(mouse))
                {
                    portal = 1;
                }
                else if (adminb.getGlobalBounds().contains(mouse))
                {
                    portal = 2;
                }
                if (id.getGlobalBounds().contains(mouse))
                {
                    info = 1;
                }
                else if (password.getGlobalBounds().contains(mouse))
                {
                    info = 2;
                }
            }
            if (const auto* text = event->getIf <Event::TextEntered >())
            {
                if (text->unicode == 8)
                {
                    if (info == 1&& !enterid.empty())
                    {
                        enterid.pop_back();
                    }
                    else if (info == 2 && !enterpassword.empty())
                    {
                        enterpassword.pop_back();
                    }
                }
                if (text->unicode >= 32 && text->unicode <= 127)
                {
                    if (info == 1)
                    {
                        enterid += static_cast<char>(text->unicode);
                    }
                    else if (info == 2)
                    {
                        enterpassword = enterpassword += static_cast<char>(text-> unicode);
                    }
                }
            }
            if (const auto* Key = event->getIf <Event::KeyPressed>())
            {
                if (Key->code == Keyboard::Key::Enter)
                {
                    accounts a;
                    bool valid = loadaccount(enterid,a) && a.password == enterpassword;
                }
            }
        } 
        if (enterid.empty() && info != 1)   
        {
            idtext.setString("Enter ID");
            idtext.setFillColor(Color(55, 55, 55));
        }
        else
        {
            idtext.setString(enterid);
            idtext.setFillColor(Color::Black);
        }
        if (enterpassword.empty() && info != 2)
        {
            passtext.setString("Enter Password");
            passtext.setFillColor(Color(55, 55, 55));
        }
        else
        {
            passtext.setString(string(enterpassword.size(), '*'));
            passtext.setFillColor(Color::Black);
        }
        windows.clear();
        windows.draw(bg);
        if (showerror== true)
        {
            windows.draw(error);
        }
        windows.draw(welcome);
        windows.draw(logint);
        windows.draw(logg);
        windows.draw(login);
        windows.draw(userb);
        windows.draw(userid);
        windows.draw(adminb);
        windows.draw(adminid);
        windows.draw(id);
        windows.draw(pass);
        windows.draw(password);
        windows.draw(logbox);
        windows.draw(log);
        windows.draw(idtext);
        windows.draw(passtext);
        windows.draw(other);
        windows.display();
    }
}
