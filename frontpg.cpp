# include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include<optional>
using namespace std;
using namespace sf;
string id;
string password;
long long balance;
void saveaccount(const string& id, const string& password, long long balance)
{
    ofstream file(id + ".txt");
    if (file.is_open())
    {
        file << id << endl;
        file << password << endl;
        file << balance << endl;
        file.close();
    }
}
bool loadaccount(const string& searchId, string& outId, string& outPass, long long& outBal)
{
    ifstream file(searchId + ".txt");
    if (file.is_open())
    {
        getline(file, outId);
        getline(file, outPass);
        file >> outBal;
        file.close();
        return true;
    }
    return false;
}
void trylogin(const string& enterid, string& enterpassword, int portal, string& curid, string& curpassword, long long& curbalance, int& page, bool& showmsg, string& amount, bool& showerror)
{
    string loadedId, p;
    long long b = 0;
    bool valid = loadaccount(enterid, loadedId, p, b) && p == enterpassword;
    if (valid == true && portal == 1 && enterid == "admin")
    {
        valid = false;
    }
    if (valid == true && portal == 2 && enterid != "admin")
    {
        valid = false;
    }
    if (valid == true)
    {
        curid = enterid;
        curpassword = p;
        curbalance = b;
        page = 1;
        showmsg = false;
        amount.clear();
        enterpassword.clear();
    }
    showerror = !valid;
}
void trysignup(string& enterid, string& enterpassword, Text& error, bool& showerror)
{
    string loadedId, p;
    long long b = 0;
    if (enterid.empty() || enterpassword.empty())
    {
        error.setString("First Type an ID and pssword");
    }
    else if (loadaccount(enterid, loadedId, p, b))
    {
        error.setString("The account aready exists");
    }
    else
    {
        saveaccount(enterid, enterpassword, 0);
        error.setString("Account made now press login in.");
    }
    showerror = true;
}
void dotransaction(Text& msg, int mode, string& amount, string& targetId, string& curid, string& curpass, long long& curb, bool& showmsg, string& lastType, long long& lastAmount, string& lastTarget)
{
    long long amt = 0;
    if (!amount.empty())
    {
        amt = stoll(amount);
    }
    if (amt <= 0)
    {
        msg.setString("First type an amount");
    }
    else if ((mode == 2 || mode == 3) && amt > curb)
    {
        msg.setString("Not enough money");
    }
    else if (mode == 3)
    {
        string recId, recPass;
        long long recBal = 0;
        if (targetId.empty())
        {
            msg.setString("Enter recipient ID");
        }
        else if (targetId == curid)
        {
            msg.setString("Cannot transfer to yourself");
        }
        else if (!loadaccount(targetId, recId, recPass, recBal))
        {
            msg.setString("Recipient account not found");
        }
        else
        {
            curb -= amt;
            saveaccount(curid, curpass, curb);
            recBal += amt;
            saveaccount(recId, recPass, recBal);
            lastType = "Transfer";
            lastAmount = amt;
            lastTarget = targetId;
            msg.setString("Transferred Rs." + to_string(amt) + " to " + targetId + " | Bal: " + to_string(curb));
            amount.clear();
            targetId.clear();
        }
    }
    else
    {
        if (mode == 1)
        {
            curb += amt;
            lastType = "Deposit";
        }
        else if (mode == 2)
        {
            curb -= amt;
            lastType = "Withdraw";
        }
        lastAmount = amt;
        lastTarget = "";
        saveaccount(curid, curpass, curb);
        string word = (mode == 1) ? "Deposite" : "Withdraw";
        msg.setString(word + " done. Balance: " + to_string(curb));
        amount.clear();
    }
    showmsg = true;
}
void addBtn(RectangleShape mb[7], Text mt[7], int i, const Font& font, float x, float y, float w, float h, float tx
    , float ty, string label)
{
    mb[i].setSize({ w, h });
    mb[i].setPosition({ x, y });
    mb[i].setFillColor({ Color(20, 130, 125) });
    mt[i].setFont(font);
    mt[i].setString(label);
    mt[i].setCharacterSize(30);
    mt[i].setPosition({ tx, ty });
    mt[i].setFillColor({ Color::White });
}
int main()
{
    RenderWindow windows(VideoMode({ 1000, 600 }), "front page!");
    Texture pic;
    pic.loadFromFile("bg.png");
    Sprite bg(pic);
    bg.setScale({ 1000.f / pic.getSize().x, 600.f / pic.getSize().y });
    Texture pic2;
    pic2.loadFromFile("bg2.png");
    Sprite bg2(pic2);
    bg2.setScale({ 1000.f / pic2.getSize().x, 600.f / pic2.getSize().y });
    Font font;
    font.openFromFile("arial.ttf");
    string tid, tpass;
    long long tbal = 0;
    if (!loadaccount("user1", tid, tpass, tbal))
    {
        saveaccount("user1", "1234", 1000000);
    }
    if (!loadaccount("user2", tid, tpass, tbal))
    {
        saveaccount("user2", "5678", 500000);
    }
    if (!loadaccount("user3", tid, tpass, tbal))
    {
        saveaccount("user3", "0000", 156000);
    }
    if (!loadaccount("admin", tid, tpass, tbal))
    {
        saveaccount("admin", "abcd", 0);
    }
    RectangleShape mb[7];
    Text mt[7] = { Text(font),Text(font), Text(font), Text(font), Text(font), Text(font),Text(font) };
    addBtn(mb, mt, 0, font, 190, 150, 300, 100, 210, 170, "Withdraw");
    addBtn(mb, mt, 1, font, 540, 150, 300, 100, 550, 170, "Deposit");
    addBtn(mb, mt, 2, font, 190, 270, 300, 100, 210, 290, "Transfer to another \n account");
    addBtn(mb, mt, 3, font, 540, 270, 300, 100, 550, 290, "Check Balance");
    addBtn(mb, mt, 4, font, 540, 380, 300, 100, 550, 400, "Generate Receipt");
    addBtn(mb, mt, 5, font, 190, 380, 300, 100, 210, 400, "View History");
    addBtn(mb, mt, 6, font, 380, 490, 300, 80, 450, 510, "Log Out");
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
    adminid.setPosition({ 300.f , 220.f });
    adminid.setFillColor({ Color::White });
    Text login(font, "Login ID:", 20);
    login.setPosition({ 120.f , 280.f });
    int info = 0;
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
    RectangleShape logbox({ 140, 30 });
    logbox.setPosition({ 190, 440 });
    logbox.setFillColor(Color::Blue);
    Text log(font, "LOGIN", 25);
    log.setPosition({ 220, 440 });
    log.setFillColor({ Color::White });
    Text other(font, "Do not have an account signup?", 20);
    other.setPosition({ 130, 480 });
    other.setFillColor(Color::White);
    Text idtext(font, "", 25);
    idtext.setPosition({ 120, 310 });
    idtext.setFillColor({ Color::Black });
    Text passtext(font, "", 25);
    passtext.setPosition({ 120, 390 });
    passtext.setFillColor({ Color::Black });
    Text error(font, "Wrong ID or password", 20);
    error.setPosition({ 150, 520 });
    error.setFillColor(Color::White);
    string curid, curpas;
    long long curb = 0;
    string lastType = "None";
    long long lastAmount = 0;
    string lastTarget = "";
    string amount;
    Text service(font, "Please select the required service", 30);
    service.setPosition({ 300.f, 80.f });
    service.setFillColor(Color::White);
    Text main(font, "", 40);
    main.setPosition({ 30, 50 });
    main.setFillColor({ Color::White });
    Text amountb(font, "Enter Amount (Rs.): ", 30);
    amountb.setPosition({ 30, 120 });
    amountb.setFillColor({ Color::White });
    RectangleShape re({ 900, 50 });
    re.setPosition({ 30, 180 });
    re.setFillColor({ Color::White });
    Text amountt(font, "", 30);
    amountt.setPosition({ 40, 185 });
    amountt.setFillColor({ Color::Black });
    string targetId;
    int activeBox = 1;
    Text targetb(font, "Recipient Account ID: ", 30);
    targetb.setPosition({ 30, 250 });
    targetb.setFillColor({ Color::White });
    RectangleShape targetRe({ 900, 50 });
    targetRe.setPosition({ 30, 300 });
    targetRe.setFillColor({ Color::White });
    Text targett(font, "", 30);
    targett.setPosition({ 40, 305 });
    targett.setFillColor({ Color::Black });
    RectangleShape b({ 300, 60 });
    b.setPosition({ 350, 380 });
    b.setFillColor({ Color(20, 130, 125) });
    Text confirm(font, "Confirm", 30);
    confirm.setPosition({ 430, 390 });
    confirm.setFillColor({ Color::White });
    RectangleShape backb({ 300, 60 });
    backb.setPosition({ 350, 460 });
    backb.setFillColor({ Color(20, 130, 125) });
    Text back(font, "Back ", 30);
    back.setPosition({ 460, 470 });
    back.setFillColor({ Color::White });
    Text msg(font, "", 24);
    msg.setPosition({ 300.f, 30.f });
    msg.setFillColor(Color::Yellow);
    bool showmsg = false;
    bool showerror = false;
    int page = 0, mode = 0;
    while (windows.isOpen())
    {
        while (const optional event = windows.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                windows.close();
            }
            if (const auto* click = event->getIf <Event::MouseButtonPressed>())
            {
                Vector2f mouse(click->position);
                if (page == 0)
                {
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
                        trylogin(enterid, enterpassword, portal, curid, curpas, curb, page, showmsg, amount, showerror);
                    }
                    else if (other.getGlobalBounds().contains(mouse))
                    {
                        trysignup(enterid, enterpassword, error, showerror);
                    }
                }
                else if (page == 1)
                {
                    if (mb[0].getGlobalBounds().contains(mouse))
                    {
                        page = 2;
                        mode = 2;
                        activeBox = 1;
                        main.setString("Withdraw");
                        amount.clear();
                        showmsg = false;
                    }
                    else if (mb[1].getGlobalBounds().contains(mouse))
                    {
                        page = 2;
                        mode = 1;
                        activeBox = 1;
                        main.setString("Deposite");
                        amount.clear();
                        showmsg = false;
                    }
                    else if (mb[2].getGlobalBounds().contains(mouse))
                    {
                        page = 2;
                        mode = 3;
                        activeBox = 1;
                        main.setString("Transfer Money");
                        amount.clear();
                        targetId.clear();
                        showmsg = false;
                    }
                    else if (mb[3].getGlobalBounds().contains(mouse))
                    {
                        msg.setString("Your balance: " + to_string(curb));
                        showmsg = true;
                    }
                    else if (mb[4].getGlobalBounds().contains(mouse)) // Generate Receipt
                {
                    if (lastType == "None")
                    {
                        msg.setString("No transaction yet to generate receipt");
                    }
                    else
                    {
                        ofstream receipt(curid + "_receipt.txt");
                        if (receipt.is_open())
                        {
                            receipt << "==================================" << endl;
                            receipt << "          BANK RECEIPT            " << endl;
                            receipt << "==================================" << endl;
                            receipt << "Account ID    : " << curid << endl;
                            receipt << "Transaction   : " << lastType << endl;
                            receipt << "Amount        : Rs. " << lastAmount << endl;
                            if (lastType == "Transfer")
                            {
                                receipt << "Recipient ID  : " << lastTarget << endl;
                            }
                            receipt << "Current Bal   : Rs. " << curb << endl;
                            receipt << "==================================" << endl;
                            receipt.close();
                            msg.setString("Receipt saved to " + curid + "_receipt.txt");
                        }
                        else
                        {
                            msg.setString("Error generating receipt");
                        }
                    }
                    showmsg = true;
                }
                else if (mb[5].getGlobalBounds().contains(mouse))
                {
                    // View History
                }
                else if (mb[6].getGlobalBounds().contains(mouse))
                {
                    page = 0;
                    enterid.clear();
                    enterpassword.clear();
                    amount.clear();
                    targetId.clear();
                    lastType = "None";
                    lastAmount = 0;
                    lastTarget.clear();
                    info = 0;
                    showerror = false;
                    showmsg = false;
                }
                }
                else
                {
                    if (re.getGlobalBounds().contains(mouse))
                    {
                        activeBox = 1;
                    }
                    else if (mode == 3 && targetRe.getGlobalBounds().contains(mouse))
                    {
                        activeBox = 2;
                    }
                    else if (b.getGlobalBounds().contains(mouse))
                    {
                        dotransaction(msg, mode, amount, targetId, curid, curpas, curb, showmsg, lastType, lastAmount, lastTarget);
                    }
                    else if (backb.getGlobalBounds().contains(mouse))
                    {
                        page = 1;
                        amount.clear();
                        targetId.clear();
                        showmsg = false;
                    }
                }
           
            }
            if (const auto* text = event->getIf<Event::TextEntered>())
            {
                char c = static_cast<char>(text->unicode);
                if (page == 0)
                {
                    if (text->unicode == 8)
                    {
                        if (info == 1 && !enterid.empty())
                        {
                            enterid.pop_back();
                        }
                        else if (info == 2 && !enterpassword.empty())
                        {
                            enterpassword.pop_back();
                        }
                    }
                    else if (text->unicode >= 32 && text->unicode <= 127)
                    {
                        if (info == 1 && isalnum(static_cast<unsigned char>(c)) && enterid.size() < 20)
                        {
                            enterid += c;
                        }
                        else if (info == 2 && enterpassword.size() < 20)
                        {
                            enterpassword += c;
                        }
                    }
                }
                else if (page == 2)
                {
                    if (text->unicode == 8)
                    {
                        if (activeBox == 1 && !amount.empty())
                        {
                            amount.pop_back();
                        }
                        else if (activeBox == 2 && !targetId.empty())
                        {
                            targetId.pop_back();
                        }
                    }
                    else if (activeBox == 1 && text->unicode >= '0' && text->unicode <= '9' && amount.size() < 9)
                    {
                        amount += c;
                    }
                    else if (activeBox == 2 && mode == 3 && isalnum(static_cast<unsigned char>(c)) && targetId.size() < 20)
                    {
                        targetId += c;
                    }
                }
            }

            if (const auto* Key = event->getIf<Event::KeyPressed>())
            {
                if (Key->code == Keyboard::Key::Enter)
                {
                    if (page == 0)
                    {
                        trylogin(enterid, enterpassword, portal, curid, curpas, curb, page, showmsg, amount, showerror);
                    }
                    else if (page == 2)
                    {
                        dotransaction(msg, mode, amount, targetId, curid, curpas, curb, showmsg, lastType, lastAmount, lastTarget);
                    }
                }
            }
        }
        if (page == 0)
        {
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
        }
        else if (page == 2)
        {
            if (amount.empty())
            {
                amountt.setString("amount");
                amountt.setFillColor(Color(55, 55, 55));
            }
            else
            {
                amountt.setString(amount);
                amountt.setFillColor(Color::Black);
            }

            if (targetId.empty())
            {
                targett.setString("recipient id");
                targett.setFillColor(Color(55, 55, 55));
            }
            else
            {
                targett.setString(targetId);
                targett.setFillColor(Color::Black);
            }
        }
        windows.clear();
        if (page == 0)
        {
            windows.draw(bg);
            if (showerror == true)
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
        }
        else if (page == 1)
        {
            windows.draw(bg2);
            windows.draw(service);
            for (int i = 0; i < 7; i++)
            {
                windows.draw(mb[i]);
                windows.draw(mt[i]);
            }
            if (showmsg == true)
            {
                windows.draw(msg);
            }
        }
        else
        {
            windows.draw(bg2);
            windows.draw(amountb);
            windows.draw(main);
            windows.draw(re);
            windows.draw(amountt);
            if (mode == 3)
            {
                windows.draw(targetb);
                windows.draw(targetRe);
                windows.draw(targett);
            }
            windows.draw(b);
            windows.draw(confirm);
            windows.draw(backb);
            windows.draw(back);
            if (showmsg == true)
            {
                windows.draw(msg);
            }
        }
        windows.display();
    }
    return 0;
}
