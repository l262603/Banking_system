#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <optional>
#include <ctime>
#include <cmath>
using namespace std;
using namespace sf;
const Color THEME_BG(238, 240, 243);
const Color THEME_BTN(11, 42, 91);
const Color THEME_NAVY(11, 42, 91);
const Color THEME_CARD(255, 255, 255);
const Color THEME_BORDER(213, 218, 226);
const Color THEME_FIELD(255, 255, 255);
const Color THEME_FIELD_BORDER(160, 170, 185);
const Color THEME_TEXT(27, 36, 52);
const Color THEME_MUTED(71, 84, 103);
const Color THEME_HINT(100, 110, 125);
const Color THEME_ERROR(200, 40, 40);
const float WIN_W = 1000.f;
const float WIN_H = 600.f;
string id;
string password;
long long balance;
void centerX(Text& t, float cx, float y)
{
    FloatRect b = t.getLocalBounds();
    t.setPosition({ floor(cx - (b.position.x + b.size.x / 2.f)), y });
}
void centerIn(Text& t, float x, float y, float w, float h)
{
    FloatRect b = t.getLocalBounds();
    t.setPosition({ floor(x + w / 2.f - (b.position.x + b.size.x / 2.f)),
                    floor(y + h / 2.f - (b.position.y + b.size.y / 2.f)) });
}
void updateView(RenderWindow& w, View& v, Vector2u size)
{
    if (size.x == 0 || size.y == 0)
    {
        return;
    }
    float windowRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
    float viewRatio = WIN_W / WIN_H;
    float sx = 1.f, sy = 1.f, px = 0.f, py = 0.f;
    if (windowRatio > viewRatio)
    {
        sx = viewRatio / windowRatio;
        px = (1.f - sx) / 2.f;
    }
    else
    {
        sy = windowRatio / viewRatio;
        py = (1.f - sy) / 2.f;
    }
    v.setViewport(FloatRect({ px, py }, { sx, sy }));
    w.setView(v);
}
void registerAccountId(const string& newId)
{
    ifstream inFile("accounts_list.txt");
    string line;
    while (getline(inFile, line))
    {
        if (line == newId)
        {
            inFile.close();
            return;
        }
    }
    inFile.close();

    ofstream outFile("accounts_list.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << newId << endl;
        outFile.close();
    }
}
void saveaccount(const string& id, const string& password, long long balance, int disabled = 0)
{
    ofstream file(id + ".txt");
    if (file.is_open())
    {
        file << id << endl;
        file << password << endl;
        file << balance << endl;
        file << disabled << endl;
        file.close();
    }
    if (id != "admin")
    {
        registerAccountId(id);
    }
}
bool loadaccount(const string& searchId, string& outId, string& outPass, long long& outBal, int& outDisabled)
{
    ifstream file(searchId + ".txt");
    if (file.is_open())
    {
        getline(file, outId);
        getline(file, outPass);
        file >> outBal;
        if (!(file >> outDisabled))
        {
            outDisabled = 0;
        }
        file.close();
        return true;
    }
    return false;
}
void trylogin(const string& enterid, string& enterpassword, int portal, string& curid, string& curpassword, long long& curbalance, int& page, bool& showmsg, string& amount, bool& showerror, Text& error)
{
    string loadedId, p;
    long long b = 0;
    int disabled = 0;
    bool valid = loadaccount(enterid, loadedId, p, b, disabled) && p == enterpassword;
    if (valid == true && portal == 1 && enterid == "admin")
    {
        valid = false;
    }
    if (valid == true && portal == 2 && enterid != "admin")
    {
        valid = false;
    }
    if (valid == true && disabled == 1)
    {
        error.setString("This account is disabled by Admin");
        showerror = true;
        return;
    }
    if (valid == true)
    {
        curid = enterid;
        curpassword = p;
        curbalance = b;
        if (enterid == "admin")
        {
            page = 4;
        }
        else
        {
            page = 1;
        }
        showmsg = false;
        amount.clear();
        enterpassword.clear();
        showerror = false;
    }
    else
    {
        error.setString("Wrong ID or password");
        showerror = true;
    }
}
void trysignup(string& enterid, string& enterpassword, Text& error, bool& showerror)
{
    string loadedId, p;
    long long b = 0;
    int disabled = 0;
    if (enterid.empty() || enterpassword.empty())
    {
        error.setString("First Type an ID and password");
    }
    else if (enterid == "admin")
    {
        error.setString("Cannot use reserved ID");
    }
    else if (loadaccount(enterid, loadedId, p, b, disabled))
    {
        error.setString("The account aready exists");
    }
    else
    {
        saveaccount(enterid, enterpassword, 0, 0);
        error.setString("Account made. Now press login in.");
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
        int recDisabled = 0;
        if (targetId.empty())
        {
            msg.setString("Enter recipient ID");
        }
        else if (targetId == curid)
        {
            msg.setString("Cannot transfer to yourself");
        }
        else if (targetId == "admin" || !loadaccount(targetId, recId, recPass, recBal, recDisabled))
        {
            msg.setString("Recipient account not found");
        }
        else if (recDisabled == 1)
        {
            msg.setString("Recipient account is disabled");
        }
        else
        {
            curb -= amt;
            saveaccount(curid, curpass, curb, 0);
            ofstream senderHist(curid + "_history.txt", ios::app);
            if (senderHist.is_open())
            {
                senderHist << "Transferred Rs. " << amt << " to " << recId << " | Bal: " << curb << endl;
                senderHist.close();
            }

            recBal += amt;
            saveaccount(recId, recPass, recBal, recDisabled);
            ofstream recHist(recId + "_history.txt", ios::app);
            if (recHist.is_open())
            {
                recHist << "Received Rs. " << amt << " from " << curid << " | Bal: " << recBal << endl;
                recHist.close();
            }

            ofstream globalHist("all_transactions.txt", ios::app);
            if (globalHist.is_open())
            {
                globalHist << curid << " transferred Rs. " << amt << " to " << recId << endl;
                globalHist.close();
            }

            lastType = "Transfer";
            lastAmount = amt;
            lastTarget = recId;
            msg.setString("Transferred Rs." + to_string(amt) + " to " + recId + " | Bal: " + to_string(curb));
            amount.clear();
            targetId.clear();
        }
    }
    else
    {
        string word;
        if (mode == 1)
        {
            curb += amt;
            lastType = "Deposit";
            word = "Deposit";
        }
        else if (mode == 2)
        {
            curb -= amt;
            lastType = "Withdraw";
            word = "Withdraw";
        }

        lastAmount = amt;
        lastTarget = "";
        saveaccount(curid, curpass, curb, 0);

        ofstream hist(curid + "_history.txt", ios::app);
        if (hist.is_open())
        {
            hist << word << " Rs. " << amt << " | Bal: " << curb << endl;
            hist.close();
        }

        ofstream globalHist("all_transactions.txt", ios::app);
        if (globalHist.is_open())
        {
            globalHist << curid << " - " << word << " Rs. " << amt << " | Bal: " << curb << endl;
            globalHist.close();
        }

        msg.setString(word + " done. Balance: " + to_string(curb));
        amount.clear();
    }
    showmsg = true;
}
void addBtn(RectangleShape mb[], Text mt[], int i, const Font& font, float x, float y, float w, float h, string label)
{
    mb[i].setSize({ w, h });
    mb[i].setPosition({ x, y });
    mb[i].setFillColor(THEME_BTN);
    mb[i].setOutlineThickness(2.f);
    mb[i].setOutlineColor(THEME_BTN);
    mt[i].setFont(font);
    mt[i].setString(label);
    mt[i].setCharacterSize(28);
    mt[i].setStyle(Text::Bold);
    mt[i].setFillColor({ Color::White });
    centerIn(mt[i], x, y, w, h);
}
int main()
{
    RenderWindow windows(VideoMode({ 1000, 600 }), "Nova Bank");
    View view(FloatRect({ 0.f, 0.f }, { WIN_W, WIN_H }));
    windows.setView(view);

    Font font;
    font.openFromFile("arial.ttf");

    string tid, tpass;
    long long tbal = 0;
    int tdis = 0;
    if (!loadaccount("user1", tid, tpass, tbal, tdis))
    {
        saveaccount("user1", "1234", 1000000, 0);
    }
    else
    {
        registerAccountId("user1");
    }
    if (!loadaccount("user2", tid, tpass, tbal, tdis))
    {
        saveaccount("user2", "5678", 500000, 0);
    }
    else
    {
        registerAccountId("user2");
    }
    if (!loadaccount("user3", tid, tpass, tbal, tdis))
    {
        saveaccount("user3", "0000", 156000, 0);
    }
    else
    {
        registerAccountId("user3");
    }
    if (!loadaccount("admin", tid, tpass, tbal, tdis))
    {
        saveaccount("admin", "abcd", 0, 0);
    }
    RectangleShape headerBar({ WIN_W, 64.f });
    headerBar.setPosition({ 0.f, 0.f });
    headerBar.setFillColor(THEME_CARD);
    RectangleShape headerLine({ WIN_W, 1.f });
    headerLine.setPosition({ 0.f, 64.f });
    headerLine.setFillColor(THEME_BORDER);
    RectangleShape logoBox({ 40.f, 40.f });
    logoBox.setPosition({ 30.f, 12.f });
    logoBox.setFillColor(THEME_NAVY);
    Text logoN(font, "N", 28);
    logoN.setStyle(Text::Bold);
    logoN.setFillColor(Color::White);
    centerIn(logoN, 30.f, 12.f, 40.f, 40.f);
    Text brand(font, "Nova Bank", 26);
    brand.setStyle(Text::Bold);
    brand.setFillColor(THEME_NAVY);
    brand.setPosition({ 82.f, 15.f });
    Text clockText(font, "", 22);
    clockText.setStyle(Text::Bold);
    clockText.setFillColor(THEME_NAVY);
    Text dateText(font, "", 14);
    dateText.setFillColor(THEME_MUTED);
    Text welcome(font, "Welcome to Nova Bank", 38);
    welcome.setStyle(Text::Bold);
    welcome.setFillColor(THEME_NAVY);
    centerX(welcome, WIN_W / 2.f, 72.f);
    Text tagline(font, "Money, made simple. Send, save and track every rupee in one place.", 17);
    tagline.setFillColor(THEME_MUTED);
    centerX(tagline, WIN_W / 2.f, 122.f);
    Text feat[3] = { Text(font, "Bank-level security", 17), Text(font, "Instant transfers", 17), Text(font, "24/7 customer support", 17) };
    CircleShape featCircle[3];
    RectangleShape armA[3], armB[3];
    {
        float total = 0.f;
        float gap = 36.f;
        for (int i = 0; i < 3; i++)
        {
            feat[i].setStyle(Text::Bold);
            feat[i].setFillColor(THEME_NAVY);
            total += 24.f + 8.f + feat[i].getLocalBounds().size.x;
        }
        total += 2.f * gap;
        float x = floor((WIN_W - total) / 2.f);
        float cy = 160.f;
        for (int i = 0; i < 3; i++)
        {
            float cx = x + 12.f;
            featCircle[i].setRadius(12.f);
            featCircle[i].setOrigin({ 12.f, 12.f });
            featCircle[i].setPosition({ cx, cy });
            featCircle[i].setFillColor(THEME_NAVY);

            armA[i].setSize({ 6.f, 3.f });
            armA[i].setOrigin({ 0.f, 1.5f });
            armA[i].setPosition({ cx - 6.f, cy });
            armA[i].setRotation(degrees(45.f));
            armA[i].setFillColor(Color::White);

            armB[i].setSize({ 11.f, 3.f });
            armB[i].setOrigin({ 0.f, 1.5f });
            armB[i].setPosition({ cx - 1.8f, cy + 4.2f });
            armB[i].setRotation(degrees(-45.f));
            armB[i].setFillColor(Color::White);

            feat[i].setPosition({ floor(x + 24.f + 8.f), cy - 12.f });
            x += 24.f + 8.f + feat[i].getLocalBounds().size.x + gap;
        }
    }
    RectangleShape logint({ 320.f, 380.f });
    logint.setPosition({ 340.f, 190.f });
    logint.setFillColor(THEME_CARD);
    logint.setOutlineThickness(1.f);
    logint.setOutlineColor(THEME_BORDER);
    int portal = 0;
    Text logg(font, "Welcome back", 28);
    logg.setStyle(Text::Bold);
    logg.setFillColor(THEME_NAVY);
    centerX(logg, 500.f, 200.f);
    Text logSub(font, "Login to your account", 15);
    logSub.setFillColor(THEME_HINT);
    centerX(logSub, 500.f, 236.f);
    RectangleShape userb({ 130, 30 });
    userb.setPosition({ 360, 265 });
    userb.setFillColor(THEME_CARD);
    userb.setOutlineThickness(2.f);
    userb.setOutlineColor(THEME_BTN);
    RectangleShape adminb({ 130, 30 });
    adminb.setPosition({ 510, 265 });
    adminb.setFillColor(THEME_CARD);
    adminb.setOutlineThickness(2.f);
    adminb.setOutlineColor(THEME_BTN);
    Text userid(font, "USER", 18);
    userid.setStyle(Text::Bold);
    userid.setFillColor(THEME_NAVY);
    centerIn(userid, 360, 265, 130, 30);
    Text adminid(font, "ADMIN", 18);
    adminid.setStyle(Text::Bold);
    adminid.setFillColor(THEME_NAVY);
    centerIn(adminid, 510, 265, 130, 30);
    Text login(font, "Login ID:", 16);
    login.setPosition({ 360.f, 306.f });
    int info = 0;
    string enterid, enterpassword;
    login.setFillColor(THEME_NAVY);
    RectangleShape id({ 280.f, 36.f });
    id.setPosition({ 360.f, 328.f });
    id.setFillColor(THEME_FIELD);
    id.setOutlineThickness(2.f);
    id.setOutlineColor(THEME_FIELD_BORDER);
    Text pass(font, "Password:", 16);
    pass.setPosition({ 360.f, 373.f });
    pass.setFillColor(THEME_NAVY);
    RectangleShape password({ 280.f, 36.f });
    password.setPosition({ 360.f, 395.f });
    password.setFillColor(THEME_FIELD);
    password.setOutlineThickness(2.f);
    password.setOutlineColor(THEME_FIELD_BORDER);
    RectangleShape logbox({ 280, 44 });
    logbox.setPosition({ 360, 448 });
    logbox.setFillColor(THEME_BTN);
    Text log(font, "Login", 24);
    log.setStyle(Text::Bold);
    log.setFillColor({ Color::White });
    centerIn(log, 360, 448, 280, 44);
    Text other(font, "Do not have an account signup?", 16);
    other.setFillColor(THEME_BTN);
    centerX(other, 500.f, 502.f);
    Text idtext(font, "", 22);
    idtext.setPosition({ 370, 332 });
    idtext.setFillColor({ THEME_TEXT });
    Text passtext(font, "", 22);
    passtext.setPosition({ 370, 399 });
    passtext.setFillColor({ THEME_TEXT });
    Text error(font, "Wrong ID or password", 16);
    error.setFillColor(THEME_ERROR);
    error.setPosition({ 360, 532 });
    string curid, curpas;
    long long curb = 0;
    string lastType = "None";
    long long lastAmount = 0;
    string lastTarget = "";
    string amount;
    RectangleShape card1({ 720.f, 432.f });
    card1.setPosition({ 140.f, 118.f });
    card1.setFillColor(THEME_CARD);
    card1.setOutlineThickness(1.f);
    card1.setOutlineColor(THEME_BORDER);
    Text service(font, "Please select the required service", 26);
    service.setStyle(Text::Bold);
    service.setFillColor(THEME_NAVY);
    centerX(service, WIN_W / 2.f, 74.f);
    RectangleShape mb[7];
    Text mt[7] = { Text(font), Text(font), Text(font), Text(font), Text(font), Text(font), Text(font) };
    addBtn(mb, mt, 0, font, 180, 138, 300, 95, "Withdraw");
    addBtn(mb, mt, 1, font, 520, 138, 300, 95, "Deposit");
    addBtn(mb, mt, 2, font, 180, 247, 300, 95, "Transfer to another \n account");
    addBtn(mb, mt, 3, font, 520, 247, 300, 95, "Check Balance");
    addBtn(mb, mt, 4, font, 520, 356, 300, 95, "Generate Receipt");
    addBtn(mb, mt, 5, font, 180, 356, 300, 95, "View History");
    addBtn(mb, mt, 6, font, 350, 463, 300, 70, "Log Out");
    mb[6].setFillColor(THEME_CARD);
    mb[6].setOutlineColor(THEME_BTN);
    mt[6].setFillColor(THEME_NAVY);
    RectangleShape card2({ 760.f, 465.f });
    card2.setPosition({ 120.f, 80.f });
    card2.setFillColor(THEME_CARD);
    card2.setOutlineThickness(1.f);
    card2.setOutlineColor(THEME_BORDER);
    Text main(font, "", 34);
    main.setStyle(Text::Bold);
    main.setFillColor(THEME_NAVY);
    Text amountb(font, "Enter Amount (Rs.): ", 22);
    amountb.setPosition({ 160, 155 });
    amountb.setFillColor(THEME_NAVY);
    RectangleShape re({ 680, 48 });
    re.setPosition({ 160, 187 });
    re.setFillColor(THEME_FIELD);
    re.setOutlineThickness(2.f);
    re.setOutlineColor(THEME_FIELD_BORDER);
    Text amountt(font, "", 24);
    amountt.setPosition({ 172, 197 });
    amountt.setFillColor({ THEME_TEXT });
    string targetId;
    int activeBox = 1;
    Text targetb(font, "Recipient Account ID: ", 22);
    targetb.setPosition({ 160, 250 });
    targetb.setFillColor(THEME_NAVY);
    RectangleShape targetRe({ 680, 48 });
    targetRe.setPosition({ 160, 282 });
    targetRe.setFillColor(THEME_FIELD);
    targetRe.setOutlineThickness(2.f);
    targetRe.setOutlineColor(THEME_FIELD_BORDER);
    Text targett(font, "", 24);
    targett.setPosition({ 172, 292 });
    targett.setFillColor({ THEME_TEXT });
    RectangleShape b({ 300, 56 });
    b.setPosition({ 350, 365 });
    b.setFillColor(THEME_BTN);
    Text confirm(font, "Confirm", 28);
    confirm.setStyle(Text::Bold);
    confirm.setFillColor({ Color::White });
    centerIn(confirm, 350, 365, 300, 56);
    RectangleShape backb({ 300, 56 });
    backb.setPosition({ 350, 435 });
    backb.setFillColor(THEME_CARD);
    backb.setOutlineThickness(2.f);
    backb.setOutlineColor(THEME_BTN);
    Text back(font, "Back", 28);
    back.setStyle(Text::Bold);
    back.setFillColor(THEME_NAVY);
    centerIn(back, 350, 435, 300, 56);
    Text msg(font, "", 22);
    msg.setFillColor(THEME_NAVY);
    Text histTitle(font, "Transaction History (Last 10)", 30);
    histTitle.setStyle(Text::Bold);
    histTitle.setFillColor(THEME_NAVY);
    centerX(histTitle, 500.f, 95.f);
    Text histText(font, "", 22);
    histText.setPosition({ 160.f, 145.f });
    histText.setFillColor(THEME_TEXT);
    Text adminTitle(font, "Admin Dashboard", 26);
    adminTitle.setStyle(Text::Bold);
    adminTitle.setFillColor(THEME_NAVY);
    centerX(adminTitle, WIN_W / 2.f, 74.f);
    RectangleShape amb[4];
    Text amtBtn[4] = { Text(font), Text(font), Text(font), Text(font) };
    addBtn(amb, amtBtn, 0, font, 180, 150, 300, 110, "View All Accounts");
    addBtn(amb, amtBtn, 1, font, 520, 150, 300, 110, "View All\nTransactions");
    addBtn(amb, amtBtn, 2, font, 180, 285, 300, 110, "Disable / Enable\nAccount");
    addBtn(amb, amtBtn, 3, font, 520, 285, 300, 110, "Log Out");
    amb[3].setFillColor(THEME_CARD);
    amb[3].setOutlineColor(THEME_BTN);
    amtBtn[3].setFillColor(THEME_NAVY);
    string adminTargetId;
    Text adminToggleTitle(font, "Disable / Enable Account", 34);
    adminToggleTitle.setStyle(Text::Bold);
    adminToggleTitle.setFillColor(THEME_NAVY);
    centerX(adminToggleTitle, 500.f, 93.f);
    Text adminInputLabel(font, "Enter Account ID: ", 22);
    adminInputLabel.setPosition({ 160, 175 });
    adminInputLabel.setFillColor(THEME_NAVY);
    RectangleShape adminInputBox({ 680, 48 });
    adminInputBox.setPosition({ 160, 207 });
    adminInputBox.setFillColor(THEME_FIELD);
    adminInputBox.setOutlineThickness(2.f);
    adminInputBox.setOutlineColor(THEME_BTN);
    Text adminInputText(font, "", 24);
    adminInputText.setPosition({ 172, 217 });
    adminInputText.setFillColor(THEME_TEXT);
    RectangleShape adminToggleBtn({ 300, 56 });
    adminToggleBtn.setPosition({ 350, 310 });
    adminToggleBtn.setFillColor(THEME_BTN);
    Text adminToggleLabel(font, "Disable / Enable", 26);
    adminToggleLabel.setStyle(Text::Bold);
    adminToggleLabel.setFillColor(Color::White);
    centerIn(adminToggleLabel, 350, 310, 300, 56);
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

            if (const auto* resized = event->getIf<Event::Resized>())
            {
                updateView(windows, view, resized->size);
            }

            if (const auto* click = event->getIf<Event::MouseButtonPressed>())
            {
                Vector2f mouse = windows.mapPixelToCoords(click->position);
                if (page == 0)
                {
                    if (userb.getGlobalBounds().contains(mouse))
                    {
                        portal = 1;
                        userb.setFillColor(THEME_BTN);
                        userid.setFillColor(Color::White);
                        adminb.setFillColor(THEME_CARD);
                        adminid.setFillColor(THEME_NAVY);
                    }
                    else if (adminb.getGlobalBounds().contains(mouse))
                    {
                        portal = 2;
                        userb.setFillColor(THEME_CARD);
                        userid.setFillColor(THEME_NAVY);
                        adminb.setFillColor(THEME_BTN);
                        adminid.setFillColor(Color::White);
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
                        error.setString("Wrong ID or password");
                        trylogin(enterid, enterpassword, portal, curid, curpas, curb, page, showmsg, amount, showerror, error);
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
                    else if (mb[4].getGlobalBounds().contains(mouse))
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
                        ifstream histFile(curid + "_history.txt");
                        if (!histFile.is_open())
                        {
                            histText.setString("No transaction history found.");
                        }
                        else
                        {
                            string lines[100];
                            int count = 0;
                            while (count < 100 && getline(histFile, lines[count]))
                            {
                                count++;
                            }
                            histFile.close();

                            if (count == 0)
                            {
                                histText.setString("No transaction history found.");
                            }
                            else
                            {
                                string displayStr = "";
                                int start = (count > 10) ? count - 10 : 0;
                                for (int i = start; i < count; i++)
                                {
                                    displayStr += lines[i] + "\n";
                                }
                                histText.setString(displayStr);
                            }
                        }
                        histTitle.setString("Transaction History (Last 10)");
                        centerX(histTitle, 500.f, 95.f);
                        page = 3;
                        showmsg = false;
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
                else if (page == 2)
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
                else if (page == 3)
                {
                    if (backb.getGlobalBounds().contains(mouse))
                    {
                        if (curid == "admin")
                        {
                            page = 4;
                        }
                        else
                        {
                            page = 1;
                        }
                    }
                }
                else if (page == 4)
                {
                    if (amb[0].getGlobalBounds().contains(mouse))
                    {
                        ifstream listFile("accounts_list.txt");
                        string accId, displayStr = "";
                        int count = 0;
                        while (getline(listFile, accId))
                        {
                            string uId, uPass;
                            long long uBal = 0;
                            int uDis = 0;
                            if (loadaccount(accId, uId, uPass, uBal, uDis))
                            {
                                string status;
                                if (uDis == 1)
                                {
                                    status = "DISABLED";
                                }
                                else
                                {
                                    status = "ACTIVE";
                                }
                                displayStr += uId + "  |  Balance: Rs. " + to_string(uBal) + "  |  Status: " + status + "\n";
                                count++;
                            }
                        }
                        listFile.close();

                        histTitle.setString("All Bank Accounts");
                        centerX(histTitle, 500.f, 95.f);
                        if (count == 0)
                        {
                            histText.setString("No accounts found.");
                        }
                        else
                        {
                            histText.setString(displayStr);
                        }
                        page = 3;
                        showmsg = false;
                    }
                    else if (amb[1].getGlobalBounds().contains(mouse))
                    {
                        ifstream allFile("all_transactions.txt");
                        if (!allFile.is_open())
                        {
                            histText.setString("No transactions recorded yet.");
                        }
                        else
                        {
                            string lines[200];
                            int count = 0;
                            while (count < 200 && getline(allFile, lines[count]))
                            {
                                count++;
                            }
                            allFile.close();

                            if (count == 0)
                            {
                                histText.setString("No transactions recorded yet.");
                            }
                            else
                            {
                                string displayStr = "";
                                int start = (count > 10) ? count - 10 : 0;
                                for (int i = start; i < count; i++)
                                {
                                    displayStr += lines[i] + "\n";
                                }
                                histText.setString(displayStr);
                            }
                        }
                        histTitle.setString("All Bank Transactions (Last 10)");
                        centerX(histTitle, 500.f, 95.f);
                        page = 3;
                        showmsg = false;
                    }
                    else if (amb[2].getGlobalBounds().contains(mouse))
                    {
                        adminTargetId.clear();
                        showmsg = false;
                        page = 5;
                    }
                    else if (amb[3].getGlobalBounds().contains(mouse))
                    {
                        page = 0;
                        enterid.clear();
                        enterpassword.clear();
                        info = 0;
                        showerror = false;
                        showmsg = false;
                    }
                }
                else if (page == 5)
                {
                    if (adminToggleBtn.getGlobalBounds().contains(mouse))
                    {
                        string uId, uPass;
                        long long uBal = 0;
                        int uDis = 0;
                        if (adminTargetId.empty())
                        {
                            msg.setString("First type an account ID");
                        }
                        else if (adminTargetId == "admin")
                        {
                            msg.setString("Cannot disable admin account");
                        }
                        else if (!loadaccount(adminTargetId, uId, uPass, uBal, uDis))
                        {
                            msg.setString("Account not found");
                        }
                        else
                        {
                            string stateWord;
                            if (uDis == 1)
                            {
                                uDis = 0;
                                stateWord = "ENABLED";
                            }
                            else
                            {
                                uDis = 1;
                                stateWord = "DISABLED";
                            }
                            saveaccount(uId, uPass, uBal, uDis);
                            msg.setString("Account '" + uId + "' is now " + stateWord);
                            adminTargetId.clear();
                        }
                        showmsg = true;
                    }
                    else if (backb.getGlobalBounds().contains(mouse))
                    {
                        page = 4;
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
                        if (info == 1 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && enterid.size() < 20)
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
                    else if (activeBox == 2 && mode == 3 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && targetId.size() < 20)
                    {
                        targetId += c;
                    }
                }
                else if (page == 5)
                {
                    if (text->unicode == 8)
                    {
                        if (!adminTargetId.empty())
                        {
                            adminTargetId.pop_back();
                        }
                    }
                    else if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && adminTargetId.size() < 20)
                    {
                        adminTargetId += c;
                    }
                }
            }
            if (const auto* Key = event->getIf<Event::KeyPressed>())
            {
                if (Key->code == Keyboard::Key::Enter)
                {
                    if (page == 0)
                    {
                        error.setString("Wrong ID or password");
                        trylogin(enterid, enterpassword, portal, curid, curpas, curb, page, showmsg, amount, showerror, error);
                    }
                    else if (page == 2)
                    {
                        dotransaction(msg, mode, amount, targetId, curid, curpas, curb, showmsg, lastType, lastAmount, lastTarget);
                    }
                    else if (page == 5)
                    {
                        string uId, uPass;
                        long long uBal = 0;
                        int uDis = 0;
                        if (adminTargetId.empty())
                        {
                            msg.setString("First type an account ID");
                        }
                        else if (adminTargetId == "admin")
                        {
                            msg.setString("Cannot disable admin account");
                        }
                        else if (!loadaccount(adminTargetId, uId, uPass, uBal, uDis))
                        {
                            msg.setString("Account not found");
                        }
                        else
                        {
                            string stateWord;
                            if (uDis == 1)
                            {
                                uDis = 0;
                                stateWord = "ENABLED";
                            }
                            else
                            {
                                uDis = 1;
                                stateWord = "DISABLED";
                            }
                            saveaccount(uId, uPass, uBal, uDis);
                            msg.setString("Account '" + uId + "' is now " + stateWord);
                            adminTargetId.clear();
                        }
                        showmsg = true;
                    }
                }
            }
        }
        time_t now = time(0);
        tm* t = localtime(&now);
        int hour = t->tm_hour;
        string ampm = "AM";
        if (hour >= 12)
        {
            ampm = "PM";
        }
        if (hour > 12)
        {
            hour = hour - 12;
        }
        if (hour == 0)
        {
            hour = 12;
        }
        string m = to_string(t->tm_min);
        string s = to_string(t->tm_sec);
        if (t->tm_min < 10)
        {
            m = "0" + m;
        }
        if (t->tm_sec < 10)
        {
            s = "0" + s;
        }
        string days[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                              "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
        clockText.setString(to_string(hour) + ":" + m + ":" + s + " " + ampm);
        dateText.setString(days[t->tm_wday] + ", " + to_string(t->tm_mday) + " " +
            months[t->tm_mon] + " " + to_string(t->tm_year + 1900));
        clockText.setPosition({ floor(970.f - clockText.getLocalBounds().size.x), 9.f });
        dateText.setPosition({ floor(970.f - dateText.getLocalBounds().size.x), 38.f });
        if (page == 0)
        {
            if (enterid.empty() && info != 1)
            {
                idtext.setString("Enter ID");
                idtext.setFillColor(THEME_HINT);
            }
            else
            {
                idtext.setString(enterid);
                idtext.setFillColor(THEME_TEXT);
            }
            if (enterpassword.empty() && info != 2)
            {
                passtext.setString("Enter Password");
                passtext.setFillColor(THEME_HINT);
            }
            else
            {
                passtext.setString(string(enterpassword.size(), '*'));
                passtext.setFillColor(THEME_TEXT);
            }
            if (info == 1)
            {
                id.setOutlineColor(THEME_BTN);
            }
            else
            {
                id.setOutlineColor(THEME_FIELD_BORDER);
            }
            if (info == 2)
            {
                password.setOutlineColor(THEME_BTN);
            }
            else
            {
                password.setOutlineColor(THEME_FIELD_BORDER);
            }
            centerX(error, 500.f, 532.f);
        }
        else if (page == 2)
        {
            if (amount.empty())
            {
                amountt.setString("amount");
                amountt.setFillColor(THEME_HINT);
            }
            else
            {
                amountt.setString(amount);
                amountt.setFillColor(THEME_TEXT);
            }

            if (targetId.empty())
            {
                targett.setString("recipient id");
                targett.setFillColor(THEME_HINT);
            }
            else
            {
                targett.setString(targetId);
                targett.setFillColor(THEME_TEXT);
            }
            centerX(main, 500.f, 93.f);
            if (activeBox == 1)
            {
                re.setOutlineColor(THEME_BTN);
            }
            else
            {
                re.setOutlineColor(THEME_FIELD_BORDER);
            }
            if (activeBox == 2)
            {
                targetRe.setOutlineColor(THEME_BTN);
            }
            else
            {
                targetRe.setOutlineColor(THEME_FIELD_BORDER);
            }
        }
        else if (page == 5)
        {
            if (adminTargetId.empty())
            {
                adminInputText.setString("Enter user ID (e.g. user1)");
                adminInputText.setFillColor(THEME_HINT);
            }
            else
            {
                adminInputText.setString(adminTargetId);
                adminInputText.setFillColor(THEME_TEXT);
            }
        }
        if (page == 1)
        {
            centerX(msg, 500.f, 562.f);
        }
        else if (page == 2 || page == 5)
        {
            centerX(msg, 500.f, 558.f);
        }

        windows.clear(THEME_BG);

        windows.draw(headerBar);
        windows.draw(headerLine);
        windows.draw(logoBox);
        windows.draw(logoN);
        windows.draw(brand);
        windows.draw(clockText);
        windows.draw(dateText);

        if (page == 0)
        {
            windows.draw(welcome);
            windows.draw(tagline);
            for (int i = 0; i < 3; i++)
            {
                windows.draw(featCircle[i]);
                windows.draw(armA[i]);
                windows.draw(armB[i]);
                windows.draw(feat[i]);
            }
            windows.draw(logint);
            windows.draw(logg);
            windows.draw(logSub);
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
            if (showerror == true)
            {
                windows.draw(error);
            }
        }
        else if (page == 1)
        {
            windows.draw(service);
            windows.draw(card1);
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
        else if (page == 2)
        {
            windows.draw(card2);
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
        else if (page == 3)
        {
            windows.draw(card2);
            windows.draw(histTitle);
            windows.draw(histText);
            windows.draw(backb);
            windows.draw(back);
        }
        else if (page == 4)
        {
            windows.draw(adminTitle);
            windows.draw(card1);
            for (int i = 0; i < 4; i++)
            {
                windows.draw(amb[i]);
                windows.draw(amtBtn[i]);
            }
        }
        else if (page == 5)
        {
            windows.draw(card2);
            windows.draw(adminToggleTitle);
            windows.draw(adminInputLabel);
            windows.draw(adminInputBox);
            windows.draw(adminInputText);
            windows.draw(adminToggleBtn);
            windows.draw(adminToggleLabel);
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
