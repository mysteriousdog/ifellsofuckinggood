#include "IOManger.h"
#include "Player.h"
#include "Util.h"

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
        return;
    }
    // TransObj* talkObj = new TransObj(1, MSG_TALK, 0);
    // memcpy(talkObj->msg, str.c_str(), str.length());
    // SeqToBin::getInstance().getBuff().push(talkObj);
    TransObj* talkObj = Util::getInstance().getMsgFromInput(move(str));
    if (talkObj != nullptr) {
        SeqToBin::getInstance().getBuff().push(talkObj);
    }
    talking = false;
}

void IOManger::handleOutputMsg()
{
    stringstream* msg;
    if (outputMsg.tryAndPop(msg)) {
        cout<<"###########################################################################################"<<endl;
        cout<<msg->str().c_str()<<endl;
        cout<<"###########################################################################################"<<endl;
        delete(msg);
    }
}