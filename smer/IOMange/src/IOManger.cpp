#ifdef CLIENT_COMPARE
#include "IOManger.h"
#include "Player.h"
#include "Util.h"
#include <sstream>

void IOManger::handleTalk()
{
    talking = true;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (ICANON);
    new_settings.c_lflag &= ECHO;// 关闭回显
    tcsetattr(0,TCSANOW,&new_settings);
    tcsetattr(0,TCSANOW,&stored_settings);
    string str;
    cout<<"Now say someyhing..."<<endl;
    cin>>str;
    getchar();
    cout<<"You say "<<str<<endl;
    if (str.length() >= MAX_TRANS_MSG_LEN) {
        cout<<"talk too much.. your message shoud inside "<< MAX_TRANS_MSG_LEN <<" words."<<endl;
        talking = false;
        return;
    }

    shared_ptr<TransObj> talkObj = Util::getInstance().getMsgFromInput(move(str));
    if (talkObj != nullptr) {
        SeqToBin::getInstance().getBuff().push(talkObj);
    }
    talking = false;
}

void IOManger::handleOutputMsg()
{
    std::shared_ptr<std::stringstream> msg;
    if ((msg = outputMsg.tryAndPop()) != nullptr) {
        cout<<"|----------------------------------------------------------------------------------------------------|"<<endl;
        cout<<msg->str().c_str()<<endl;
        cout<<"|----------------------------------------------------------------------------------------------------|"<<endl;
    }
}


bool IOManger::tryLoginFirst() {
    if (!Player::getInstance().isLogined()) {
        auto ss = make_shared<stringstream>();
        (*ss)<<"You should login first.(if not regin -- regin and login first to use the function!)\n";
        (*ss)<<"Try Regin ==> Regin@yourname|yourpassword\n";
        (*ss)<<"Try Login ==> Login@yourname|yourpassword\n";
        (*ss)<<"(-_-)\n";
        auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
        SeqToBin::getInstance().putSysMsg(sysObj);
        return true;
    }
    return false;
}
#endif