/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "Interface.h"


namespace BOI {


const QStringList Interface::GetProfile(ProfileType type)
{
    QStringList list;

    if (type & ProfileType_Emitters)
    {
        int numEmitters = NumEmitters();

        for (int i=0; i < numEmitters; i++)
        {
            QString uuid = Emitters()[i].uuid;
            InsertString(list, uuid);
        }
    }


    if (type & ProfileType_Receivers)
    {
        int numReceivers = NumReceivers();

        for (int i=0; i < numReceivers; i++)
        {
            QString uuid = Receivers()[i].uuid;
            InsertString(list, uuid);
        }
    }


    if (type & ProfileType_Callers)
    {
        int numCallers = NumCallers();

        for (int i=0; i < numCallers; i++)
        {
            QString uuid = Callers()[i].uuid;
            InsertString(list, uuid);
        }
    }


    if (type & ProfileType_FuncSets)
    {
        int numFuncSets = NumFuncSets();

        for (int i=0; i < numFuncSets; i++)
        {
            QString uuid = FuncSets()[i].uuid;
            InsertString(list, uuid);
        }
    }

    return list;
}


int Interface::GetCaller(const QString& uuid)
{
    int numCallers = NumCallers();

    for (int i=0; i < numCallers; i++)
    {
        if (uuid == Callers()[i].uuid)
        {
            return i;
        }
    }

    return -1;
}


int Interface::GetFuncSet(const QString& uuid)
{
    int numFuncSets = NumFuncSets();

    for (int i=0; i < numFuncSets; i++)
    {
        if (uuid == FuncSets()[i].uuid)
        {
            return i;
        }
    }

    return -1;
}


int Interface::GetEmitter(const QString& uuid)
{
    int numEmitters = NumEmitters();

    for (int i=0; i < numEmitters; i++)
    {
        if (uuid == Emitters()[i].uuid)
        {
            return i;
        }
    }

    return -1;
}


int Interface::GetReceiver(const QString& uuid)
{
    int numReceivers = NumReceivers();

    for (int i=0; i < numReceivers; i++)
    {
        if (uuid == Receivers()[i].uuid)
        {
            return i;
        }
    }

    return -1;
}


QString Interface::CallerUuid(int caller)
{
    if ((caller >= 0) && (caller < NumCallers()))
    {
        return Callers()[caller].uuid;
    }

    return QString();
}


QString Interface::FuncSetUuid(int funcSet)
{
    if ((funcSet >= 0) && (funcSet < NumFuncSets()))
    {
        return FuncSets()[funcSet].uuid;
    }

    return QString();
}


QString Interface::EmitterUuid(int emitter)
{
    if ((emitter >= 0) && (emitter < NumEmitters()))
    {
        return Emitters()[emitter].uuid;
    }

    return QString();
}


QString Interface::ReceiverUuid(int receiver)
{
    if ((receiver >= 0) && (receiver < NumReceivers()))
    {
        return Receivers()[receiver].uuid;
    }

    return QString();
}


ReceiverFunc Interface::GetReceiverFunc(int receiver)
{
    if ((receiver >= 0) && (receiver < NumReceivers()))
    {
        return Receivers()[receiver].func;
    }

    return NULL;
}


int Interface::NumFuncSetFuncs(int funcSet)
{
    if ((funcSet >= 0) && (funcSet < NumFuncSets()))
    {
        return FuncSets()[funcSet].numFuncs;
    }

    return 0;
}


const FuncSetFunc* Interface::GetFuncSetFuncs(int funcSet)
{
    if ((funcSet >= 0) && (funcSet < NumFuncSets()))
    {
        return FuncSets()[funcSet].funcs;
    }

    return NULL;
}


void Interface::InsertString(QStringList& list, QString& string)
{
    for (int i=0; i < list.size(); i++)
    {
        if (list.at(i) > string)
        {
            list.insert(i, string);
            return;
        }
    }

    list.append(string);
}


} // namespace BOI

