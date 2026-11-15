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
void addBtn(vector <RectangleShape>& mb, vector <text> &mt , const font& font, float x, float y, float w , float h, float tx
     , float ty, string label)
{
    RectangleShape re({ w, h });
    re.setPosition({ x, y });
    re.setFillColor({ Color(20, 130, 125) });
    mb.push_back(re);
    Text t(font, label, 30);
    t.setPosition({ tx, ty });
    t.setFillColor({ Color::White });
    mt.push_back(t);
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
    vector<RectangleShape>mb;
    vector <Text> mt;
    addBtn(mb,mt, font,190, 150, 300, 100, 210, 170, "Withdraw");                      
    addBtn(mb, mt, font,540, 150, 300, 100, 550, 170, "Deposit");
    addBtn(mb, mt, font,190, 270, 300, 100, 210, 290, "Transfer to another \n account");
    addBtn(mb, mt, font,540, 270, 300, 100, 550, 290, "Check Balance");
    addBtn(mb, mt, font,540, 380, 300, 100, 550, 400, "Generate Receipt");
    addBtn(mb, mt, font,190, 380, 300, 100, 210, 400, "View History");
    addBtn(mb, mt, font,380, 490, 300, 80, 450, 510, "Log Out");
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
    Text main(font, "", 40);
   	main.setPosition({ 30, 50 });
   	main.setFillColor({ Color::White });
   	Text amount(font, "Enter Amount (Rs.): ", 30);
   	amount.setPosition({ 30, 120 });
   	amount.setFillColor({ Color::White });
   	RectangleShape re({ 900, 50 });
   	re.setPosition({ 30, 200 });
   	re.setFillColor({ Color::White });
   	RectangleShape b({ 300, 60 });
   	b.setPosition({ 350, 300 });
   	b.setFillColor({ Color(20, 130, 125) });
   	Text confirm(font, "Confirm", 30);
   	confirm.setPosition({ 430, 310 });
   	confirm.setFillColor({ Color::White });
   	RectangleShape backb({ 300, 60 });
   	backb.setPosition({ 350, 390 });
   	backb.setFillColor({ Color(20, 130, 125) });
   	Text back(font, "Back ", 30);
   	back.setPosition({ 460, 400 });
   	back.setFillColor({ Color::White });
    Text msg(font, "", 24);
    msg.setPosition({ 300.f, 30.f });
    msg.setFillColor(Color::Yellow);
    bool showmsg;
    bool showerror = false;
    int page =0, mode=0;
    auto trysignup = [&]()
    {
           accounts a;
           if (enterid.empty() || enterpassword.empty())
           {
               error.setString("First Type an ID and pssword");
           }
           else if (loadaccount(enterid, a))
           {
               error.setString("The account aready exists");
           }
           else
           {
               saveaccount({ enterid, enterpassword, 0 });
               error.setString("Account made now press login in.");
           }
     };
     auto dotransaction = [&]()
        {
            long long amt = 0;
            if (amount.empty())
            {
                amt = 0;
            }
            else
            {
                amt = stoll(amount);
            }
            if (amt <= 0)
            {
                msg.setString("First type an amount");
            }
            else if (mode == 2 && amt > current.balance)
            {
                msg.setString("Not enough money");
            }
            else
            {
                if (mode == 1)
                {
                    current.balance += amt;
                }
                else if (mode == 2)
                {
                    current.balance -= amt;
                }
                saveaccount(current);
                string word;
                if (mode == 1)
                {
                    word = "Deposite";
                }
                else if (mode == 2)
                {
                    word = "Withdraw";
                }
                msg.setString(word + " done. Balance" + to_string(current.balance));
                amount.clear();
            }
            showmsg = true;
        };
      auto trylogin = [&]()
     {
         accounts a;
         bool valid = loadaccount(enterid, a) && a.password == enterpassword;
         if (valid == true && portal == 1 && a.id == "admin")
         {
             valid == false;
         }
         if (valid == true && portal == 2 && a.id != "admin")
         {
             valid = false;
         }
         if (valid == true)
         {
             current = a;
             page = 1;
             showmsg = false;
             amount.clear();
             enterpassword.clear();
         }
         showerror = !valid;
     };
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
                   userb.setFillColor(Color::Blue);
                   adminb.setFillColor(Color(0, 0, 120));
                }
                else if (adminb.getGlobalBounds().contains(mouse))
                {
                  portal = 2;
                  userb.setFillColor(Color(0, 0, 120));
                  adminb.setFillColor(Color::Blue);
                }
                if (id.getGlobalBounds().contains(mouse))
                {
                    info = 1;
                }
                else if (password.getGlobalBounds().contains(mouse))
                {
                    info = 2;
                }
             
                else if (logbox.getGlobalBounds().contains(mouse))
                {
                    trylogin;
                }
                else if (other.getGlobalBounds().contains(mouse))
                {
                    trysignup;
                }
            }
            else if (page == 1)
            {
                if (mb[0].getGlobalBounds().contains(mouse))
                {
                    page = 2;
                    mode = 2;
                    main.setString("Withdraw");
                    amount.clear();
                    showmsg = false;
                }
                else if (mb[1].getGlobalBounds().contains(mouse))
                {
                    page = 2;
                    mode = 1;
                    main.setString("Deposite");
                    amount.clear();
                    showmsg = false;
                }
                else if (mb[2].getGlobalBounds().contains(mouse))
                {
                    msg.setString("Your balance: " + to_string(current.balance));
                    showmsg = true;
                }
                else if (mb[3].getGlobalBounds().contains(mouse)||
                    (mb[4].getGlobalBounds().contains(mouse)) ||
                    (mb[5].getGlobalBounds().contains(mouse)))
                {
                    // to be declared    
                }
                else if (mb[6].getGlobalBounds().contains(mouse))
                {
                     page = 0;
                     enterid.clear();
                     enterpassword.clear();
                     amount.clear();
                     info = 0;
                     showerror = false;
                     showmsg = false;
            
                }
            }
            else
            {
                if (b.getGlobalBounds().contains(mouse))
                {
                    dotransaction;
                }
                else if (backb.getGlobalBounds().contains(mouse))
                {
                    page = 1;
                    amount.clear();
                    showmsg = false;
                }
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
                       else if (logbox.getGlobalBounds().contains(mouse))
                    {
                       trylogin;
                    }
                    else if (other.getGlobalBounds().contains(mouse))
                    {
                         trysignup;
                    }
                }
            }
            if (const auto* Key = event->getIf <Event::KeyPressed>())
            {
                if (Key->code == Keyboard::Key::Enter)
                {
                  if (Key->code == Keyboard::Key::Enter)
                 {
                   trylogin;
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
