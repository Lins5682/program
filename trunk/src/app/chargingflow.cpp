#include "chargingflow.h"

IdletimeState::IdletimeState() : ChargingFlowState(IdleTime)
{
}

QString IdletimeState::isValidCard(QString* number)
{
    return "OK";
}

ChargingFlowState* IdletimeState::handle(ChargingFlow* flow, int messageType, void* messageData)
{
    switch(messageType)
    {
    case VerifyIcCard:
    {
        if(messageData)
        {
            // 验证卡片的有效性
            QString error;
            if ((error = isValidCard((QString*)messageData)) == "OK")
            {
                return new VerificationState;
            }
            flow->generalHandleFromInner(NotifyIcCardVerificationResults, &error);
        }
        break;
    }
    }
    return 0;
}

VerificationState::VerificationState(): ChargingFlowState(Verifying)
{
}

QString VerificationState::isValidPassword(QString* password)
{
    return "OK";
}

ChargingFlowState* VerificationState::handle(ChargingFlow* flow, int messageType, void* messageData)
{
    switch(messageType)
    {
    case VerifyPasswd:   // 用户确认验证；
    {
        if(messageData != 0)
        {
            QString error = isValidPassword((QString*)messageData);
            if("OK" == error)
            {
                flow->generalHandleFromInner(VerifyPasswd, messageData);
            }
            else
            {
                flow->generalHandleFromInner(NotifyPasswdVerificationResults, &error);
            }
        }
        break;
    }
    case CancelPasswdVerification:   //  用户取消验证；
    {
        return new IdletimeState;
    }
    case NotifyPasswdVerificationResults:   //  远端完成用户验证；
    {
        if(messageData != 0)
        {
            if(*(QString*)messageData != "OK")
            {
                flow->generalHandleFromInner(NotifyPasswdVerificationResults, messageData);
            }
            else
            {
                return new ConfigurationState();
            }
        }
        break;
    }
    }
    return 0;
}

ConfigurationState::ConfigurationState() : ChargingFlowState(Configurating)
{
}

ChargingFlowState* ConfigurationState::handle(ChargingFlow* flow, int messageType, void* messageData)
{
    switch(messageType)
    {
    case Config:                    //确认配置，跳转至“准备”
        return new LaunchState;
    case CancelConfiguration:       //取消配置，跳转至“空闲”
        return new IdletimeState;
    }
    return 0;
}

LaunchState::LaunchState():ChargingFlowState(Launching)
{
}

void LaunchState::enter(ChargingFlow* flow)
{
    flow->generalHandleFromInner(Launch, 0);
}

ChargingFlowState* LaunchState::handle(ChargingFlow* flow, int messageType, void* messageData)
{
    switch(messageType)
    {
    case NotifyConnectionResults:
    {
        flow->generalHandleFromInner(NotifyConnectionResults, 0);
        break;
    }
    case NotifyLaunchResults:
    {
        if((*(QString*)messageData) != "OK")
        {
            return new AccountState;
        }
        else
        {
            return new OperationState;
        }
        break;
    }
    }
    return 0;
}

OperationState::OperationState():ChargingFlowState(Operating)
{
}

QString OperationState::isValidCard(QString* number)
{
    return "OK";
}

ChargingFlowState* OperationState::handle(ChargingFlow* flow, int messageType, void* messageData)
{
    switch(messageType)
    {
    //检测到IC卡，如果IC卡卡片是有效的，停止充电；
    case VerifyIcCard:
    {
        if(messageData)
        {
            if((isValidCard((QString*)messageData)) == "OK")
            {
                return  new TerminationState;
            }
        }
        break;
    }
    //得到已经终止的信息，进行结算；
    case NotifyTerminationResults:
        return new AccountState();
    }
    return 0;
}

TerminationState::TerminationState():ChargingFlowState(Terminating)
{
}

void TerminationState::enter(ChargingFlow* flow)
{
    flow->generalHandleFromInner(Terminate, 0);
}

ChargingFlowState* TerminationState::handle(ChargingFlow* flow, int messageType, void* messageData)
{
    switch(messageType)
    {
    //得到已经终止的信息，进行结算；
    case NotifyTerminationResults:
    {
        return new AccountState();
    }
    }
    return 0;
}

/*
 *  结算阶段
 */
AccountState::AccountState():ChargingFlowState(AccountClosing)
{
}

void AccountState::enter(ChargingFlow* flow)
{
    //上传结算信息至远端，等待远端操作、确认；
    flow->generalHandleFromInner(CloseAccount, 0);
}

ChargingFlowState* AccountState::handle(ChargingFlow* flow,
                                        int messageType,
                                        void* messageData)
{
    switch(messageType)
    {
    //账单已经完成，显示账单信息
    case NotifyAccountClosingResults:
    {
        if(messageData)
        {
            flow->generalHandleFromInner(NotifyAccountClosingResults, messageData);
        }
        break;
    }
    case EnsureAccountClosing:              //账单已经确认，进入闲置状态，等待；
    {
        return new IdletimeState;
    }
    }
    return 0;
}

void ChargingFlow::generalHandleFromOuter(int messageType, void* messageData)
{
    ChargingFlowState* nextState = currState->handle(this, messageType, messageData);
    if (nextState)
    {
        currState->exit(this);
        nextState->enter(this);
        delete currState;
        emit stateChanged((currState = nextState)->getID());
    }
}

void ChargingFlow::ifAliveHandleFromOuter(int messageType, void* messageData)
{
    if(!alive)
    {
        return;
    }
    generalHandleFromOuter(messageType, messageData);
}

void ChargingFlow::generalHandleFromInner(int messageType, void* messageData)
{
    switch(messageType)
    {
    case ChargingFlowState::NotifyIcCardVerificationResults:
        emit icCardVerified(*(QString*)messageData);
        break;
    case ChargingFlowState::NotifyPasswdVerificationResults:
        emit passwdVerified(*(QString*)messageData);
        break;
    case ChargingFlowState::VerifyPasswd:
        emit passwdVerificationDetermined(*(QString*)messageData);
        break;
    case ChargingFlowState::NotifyConnectionResults:
        emit connected(*(QString*)messageData);
        break;
//    case ChargingFlowState::Launch:
//        emit launchDetermined();
//        break;
    case ChargingFlowState::NotifyLaunchResults:
        emit launched(*(QString*)messageData);
        break;
//    case ChargingFlowState::Terminate:
//        emit terminationDetemined();
//        break;
    case ChargingFlowState::NotifyTerminationResults:
        emit terminated(*(QString*)messageData);
        break;
    case ChargingFlowState::NotifyAccountClosingResults:
        emit accountClosed();
        break;
    case ChargingFlowState::CloseAccount:
        emit accountClosingDetermined();
        break;
    }
}

//void ChargingFlow::generalHandle(int messageType, void *messageData)
//{
//    switch(messageType)
//    {
//    case ChargingFlowState::LaunchByRemoter:
//    case ChargingFlowState::ConfirmVerification:
//    case ChargingFlowState::CalloffVerification:
//    case ChargingFlowState::CalloffConfiguration:
//    case ChargingFlowState::EnsureConfiguration:
//    case ChargingFlowState::CardVerification:
//    case ChargingFlowState::TerminateByRemoter:
//    case ChargingFlowState::AccountConfirmed:
//    {
//        ChargingFlowState* nextState = currState->handle(this, messageType, messageData);
//        if (nextState)
//        {
//            currState->exit();
//            nextState->enter();
//            delete currState;
//            emit stateChanged((currState = nextState)->getID());
//        }
//        break;
//    }
//    case ChargingFlowState::Verified:
//    {
//        break;
//    }
//    case ChargingFlowState::CardVerified:
//    {
//        emit CardVerified((QString*)messageData);
//        break;
//    }
//    case ChargingFlowState::PasswdVerified:
//    {
//        emit passwdVerified((QString*)messageData);
//        break;
//    }
//    case ChargingFlowState::Connected:
//    {
//        emit connected();
//        break;
//    }
//    case ChargingFlowState::LaunchCompleted:
//    {
//        emit launched((QString*)messageData);
//        break;
//    }
//    case ChargingFlowState::Terminated:
//    {
//        break;
//    }
//    case ChargingFlowState::AccountCompleted:
//    {
//        break;
//    }
//    }
//}

//void ChargingFlow::generalHandle(int messageType, void* messageData)
//{
//    ChargingFlowState* nextState = currState->handle(this, messageType, messageData);
//    if(0 != nextState)
//    {
//        nextState->enter();
//        currState->exit();
//        delete currState;
//        emit stateChanged((currState = nextState)->getID());
//    }
//}

void ChargingFlow::resetActive(int id)
{
    // 激活本流程
    if(!alive && this->id == id)
    {
        alive = true;
        emit stateChanged(currState->getID());
    }
    // 激活其他流程，设置本流程为非激活流程
    if(alive && this->id != id)
    {
        alive = false;
    }

}

//void ChargingFlow::ifAliveHandle(int messageType, void* messageData)
//{
//    if(!alive)
//    {
//        return;
//    }
//    generalHandleFromOuter(messageType,
//                           messageData);
//}

void ChargingFlow::verifyIcCard(QString number)
{
    ifAliveHandleFromOuter(ChargingFlowState::VerifyIcCard,
                           &number);
}

void ChargingFlow::verifyPasswd(QString passwd)
{
    ifAliveHandleFromOuter(ChargingFlowState::VerifyPasswd,
                           &passwd);
}

void ChargingFlow::cancelPasswdVerification()
{
    ifAliveHandleFromOuter(ChargingFlowState::CancelPasswdVerification,
                           0);
}

void ChargingFlow::notifyPasswdVerificationResults(QString results)
{
    generalHandleFromOuter(ChargingFlowState::NotifyPasswdVerificationResults,
                           &results);
}

void ChargingFlow::config()
{
    ifAliveHandleFromOuter(ChargingFlowState::Config,
                           0);
}

void ChargingFlow::cancelConfiguration()
{
    ifAliveHandleFromOuter(ChargingFlowState::CancelConfiguration,
                           0);
}

void ChargingFlow::notifyConnectionResults(QString results)
{
    generalHandleFromOuter(ChargingFlowState::NotifyConnectionResults,
                           &results);
}

void ChargingFlow::launch(int whichOne, QString serialNumber)
{
    if(whichOne != getID())
    {
        return;
    }

    generalHandleFromOuter(ChargingFlowState::Launch,
                           &serialNumber);
}

void ChargingFlow::notifyLaunchResults(QString results)
{
    generalHandleFromOuter(ChargingFlowState::NotifyLaunchResults,
                           &results);
}

void ChargingFlow::terminate(int whichOne)
{
    if(whichOne != getID())
    {
        return;
    }

    generalHandleFromOuter(ChargingFlowState::Terminate,
                           0);
}

void ChargingFlow::notifyTerminationResults(QString results)
{
    generalHandleFromOuter(ChargingFlowState::NotifyTerminationResults,
                           &results);
}

void ChargingFlow::notifyAccountClosingResults(QString results)
{
    generalHandleFromOuter(ChargingFlowState::NotifyAccountClosingResults,
                           &results);
}

void ChargingFlow::finishAccountClosing()
{
    generalHandleFromOuter(ChargingFlowState::EnsureAccountClosing,
                           0);
}

