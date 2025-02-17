#include <assert.h>
#include <math.h>
#include <time.h>

#include <iostream>

#include "openfsm.h"

using namespace open;
enum FmsState {
    StateNormal = 100, // 状态：正常
    StateChecking,     // 状态：扫码中
    StatePaying,       // 状态：验证支付状态
    StateIssuing,      // 状态：发卡
    StatePrinting,     // 状态：打印
    StateFinishing,    // 状态：结束
    StateFail
};
enum EFMSState {
    EStateTest = 100,
    EStateLaunch = 101,
    EStateRecycle = 102,
    EStateFailure = 103,
};
struct StarShip {
    OpenFSM fsm;
    void start()
    {
        std::srand((int)time(NULL));
        fsm.setCustom(this);
        // or openFSM_.setStates({ "StateTest", "StateLaunch", "StateRecycle", "StateFailure" });
        fsm.setStates({StateNormal, StateFinishing,
                       StatePrinting,
                       StateIssuing,
                       StatePaying,
                       StateChecking, StateFail});
        fsm.enterState(StateNormal);
    }
    //    bool testEngine() { return std::rand() % 3 == 0; }
    bool testEngine() { return true; }
    bool testTank() { return true; }
    bool fireUp() { return true; }
    bool launch() { return true; }
    bool update()
    {
        fsm.update();
        return fsm.focusEState() == StateFinishing;
    }
};

class BaseAction : public OpenFSMAction {
    void enter(OpenFSM& fsm) const override
    {
        std::cout << " ->enter \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
    }
    void exit(OpenFSM& fsm) const override
    {
        std::cout << " ->exit \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
    }
};

// StateTest
class ActionTestEngine : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        if (fsm.custom<StarShip>()->testEngine()) {
            printf("==>StarShip Engine Ok\n");
            fsm.nextAction();
        } else {
            printf("==>StarShip Engine Bad\n");
            fsm.nextState(EStateFailure);
        }
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};

class ActionTestTank : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        if (fsm.custom<StarShip>()->testTank()) {
            printf("==>StarShip Tank Ok\n");
            fsm.nextAction();
        } else {
            printf("==>StarShip Tank Bad\n");
            fsm.nextState(EStateFailure);
        }
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};

// StateLaunch
class ActionFireUp : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        if (fsm.custom<StarShip>()->fireUp()) {
            printf("==>StarShip FireUp Ok\n");
            fsm.nextAction();
        } else {
            printf("==>StarShip FireUp Failed\n");
            fsm.nextState(EStateFailure);
        }
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};
class ActionLaunch : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        if (fsm.custom<StarShip>()->launch()) {
            printf("==>StarShip Launch Ok\n");
            fsm.nextAction();
        } else {
            printf("==>StarShip Launch Failed\n");
            fsm.nextState(EStateFailure);
        }
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};
class ActionLaunch1 : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        if (fsm.custom<StarShip>()->launch()) {
            printf("==>StarShip Launch1 Ok\n");
            fsm.nextAction();
        } else {
            printf("==>StarShip Launch1 Failed\n");
            fsm.nextState(EStateFailure);
        }
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};

// StateRecycle
class ActionReturn : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        printf("==>Congratulation! Complete mission!\n");
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};

// StateFailure
class ActionFailure : public BaseAction {
    void enter(OpenFSM& fsm) const override
    {
        printf("[%s.%s] enter\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
    void update(OpenFSM& fsm) const override
    {
        printf("[%s.%s] update\n", fsm.focusStateName().c_str(), actionName_.c_str());
        printf("==>StarShip Launch again\n");
        fsm.nextState(EStateTest);
    }
    void exit(OpenFSM& fsm) const override
    {
        printf("[%s.%s] exit\n", fsm.focusStateName().c_str(), actionName_.c_str());
    }
};

//  ===============================================================
class ActionConfirmPay : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};

// StateChecking -> StatePaying
class ActionCheckQr : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};

// StatePaying -> StateIssuing
class ActionVerifyPay : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};

// StateIssuing -> StatePriting
class ActionEjectCard2RwArea : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};

// StateIssuing -> StatePriting
class ActionWriteCard : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};

// StateIssuing -> StatePriting
class ActionEjectCard2Outside : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};

// StateIssuing -> StatePriting
class ActionPrintReceipt : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        if (true) {
            fsm.nextAction();
        }
    }
};
// StateFinishing -> StateNormal
class ActionFinish : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        std::cout << "end!!" << std::endl;
    }
};
class ActionFail : public BaseAction {
    void update(OpenFSM& fsm) const override
    {
        std::cout << " ->update \t:" << fsm.focusStateName() << " - " << actionName_ << std::endl;
        printf("==>StarShip Launch again\n");
        std::cout << "end!!" << std::endl;
        fsm.nextState(StateNormal);
    }
};

int main()
{
    OpenFSM::RegisterAction<ActionConfirmPay>("ActionConfirmPay");
    OpenFSM::RegisterAction<ActionCheckQr>("ActionCheckQr");
    OpenFSM::RegisterAction<ActionVerifyPay>("ActionVerifyPay");
    OpenFSM::RegisterAction<ActionEjectCard2RwArea>("ActionEjectCard2RwArea");
    OpenFSM::RegisterAction<ActionWriteCard>("ActionWriteCard");
    OpenFSM::RegisterAction<ActionEjectCard2Outside>("ActionEjectCard2Outside");
    OpenFSM::RegisterAction<ActionPrintReceipt>("ActionPrintReceipt");
    OpenFSM::RegisterAction<ActionFinish>("ActionFinish");
    OpenFSM::RegisterAction<ActionFail>("ActionFail");

    OpenFSM::RegisterState("StateNormal", {"ActionConfirmPay"}, StateNormal);
    OpenFSM::RegisterState("StateChecking", {"ActionCheckQr", "ActionWriteCard"}, StateChecking);
    OpenFSM::RegisterState("StatePaying", {"ActionVerifyPay"}, StatePaying);
    OpenFSM::RegisterState("StateIssuing", {"ActionEjectCard2RwArea"}, StateIssuing);
    OpenFSM::RegisterState("StatePrinting", {"ActionPrintReceipt"}, StatePrinting);
    OpenFSM::RegisterState("StateFinishing", {"ActionFinish"}, StateFinishing);
    OpenFSM::RegisterState("StateFail", {"ActionFail"}, StateFail);

    OpenFSM::RegisterRelation("StateNormal", {"StateChecking", "StateFail"});
    OpenFSM::RegisterRelation("StateChecking", {"StatePaying", "StateFail"});
    OpenFSM::RegisterRelation("StatePaying", {"StateIssuing", "StateFail"});
    OpenFSM::RegisterRelation("StateIssuing", {"StatePrinting", "StateFail"});
    OpenFSM::RegisterRelation("StatePrinting", {"StateFinishing", "StateFail"});

    //    OpenFSM::RegisterAction<ActionTestEngine>("ActionTestEngine");
    //    OpenFSM::RegisterAction<ActionTestTank>("ActionTestTank");
    //    OpenFSM::RegisterAction<ActionFireUp>("ActionFireUp");
    //    OpenFSM::RegisterAction<ActionLaunch>("ActionLaunch");
    //    OpenFSM::RegisterAction<ActionLaunch1>("ActionLaunch1");
    //    OpenFSM::RegisterAction<ActionReturn>("ActionReturn");
    //    OpenFSM::RegisterAction<ActionFailure>("ActionFailure");
    //
    //    OpenFSM::RegisterState("StateTest", {"ActionTestEngine", "ActionLaunch", "ActionTestTank"}, EStateTest);
    //    OpenFSM::RegisterState("StateLaunch", {"ActionFireUp", "ActionLaunch1"}, EStateLaunch);
    //    OpenFSM::RegisterState("StateRecycle", {"ActionReturn"}, EStateRecycle);
    //    OpenFSM::RegisterState("StateFailure", {"ActionFailure"}, EStateFailure);
    //
    //    OpenFSM::RegisterRelation("StateTest", {"StateLaunch", "StateFailure"});
    //    OpenFSM::RegisterRelation("StateLaunch", {"StateRecycle", "StateFailure"});
    //
    StarShip starShip;
    starShip.start();
    int count = 1000;

    while (count-- > 0) {
        if (starShip.update())
            break;
    }
    return getchar();
}
