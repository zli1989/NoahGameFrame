/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NFI_EVENT_MODULE_H
#define NFI_EVENT_MODULE_H

#include <iostream>
#include "NFIModule.h"


enum NFEventDefine
{
	NFED_UNKNOW = 0,
	////////////////Has Self---logic//////////////////////////////////////////////////////////
	//arg[0]:resultID[EGameErrorCode]
	NFED_ON_GENERAL_MESSAGE,

	////////////////core events for NF--start////////////////////////////////////////////////////////
	//scene swap
	//common property
	//common record

	////////////////core events for NF--end//////////////////////////////////////////////////////////
	//arg[0]:x,y,z
	NFED_ON_CLIENT_REQUIRE_MOVE,
	NFED_ON_CLIENT_MOVE_RESULT,

	//arg[0]:itemID,TargetObject
	NFED_ON_CLIENT_REQUIRE_USE_ITEM,

	//arg[0]:itemID, fx, fy, fz, TargetObjectList
	NFED_ON_CLIENT_REQUIRE_USE_ITEM_POS,


	NFED_ON_NOTICE_ECTYPE_AWARD,

};

class NFIEventModule
    : public NFIModule
{
public:
protected:

	typedef std::function<int(const NFGUID&, const NFEventDefine, const NFDataList&)> OBJECT_EVENT_FUNCTOR;
	typedef std::function<int(const NFEventDefine, const NFDataList&)> MODULE_EVENT_FUNCTOR;

	typedef NF_SHARE_PTR<OBJECT_EVENT_FUNCTOR> OBJECT_EVENT_FUNCTOR_PTR;//EVENT
	typedef NF_SHARE_PTR<MODULE_EVENT_FUNCTOR> MODULE_EVENT_FUNCTOR_PTR;//EVENT

public:
	// only be used in module
    virtual bool DoEvent(const NFEventDefine nEventID, const NFDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const NFEventDefine nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const NFEventDefine nEventID) = 0;

	template<typename BaseType>
	bool AddEventCallBack(const NFEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const NFEventDefine, const NFDataList&))
	{
		MODULE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		MODULE_EVENT_FUNCTOR_PTR functorPtr(new MODULE_EVENT_FUNCTOR(functor));
		return AddEventCallBack(nEventID, functorPtr);
	}
    ///////////////////////////////////////////////////////////////////////////////////////////////
	// can be used for object
    virtual bool DoEvent(const NFGUID self, const NFEventDefine nEventID, const NFDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const NFGUID self,const NFEventDefine nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const NFGUID self,const NFEventDefine nEventID) = 0;
	virtual bool RemoveEventCallBack(const NFGUID self) = 0;
	
	template<typename BaseType>
	bool AddEventCallBack(const NFGUID& self, const NFEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const NFEventDefine, const NFDataList&))
	{
		OBJECT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		OBJECT_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_EVENT_FUNCTOR(functor));
		return AddEventCallBack(self, nEventID, functorPtr);
	}

protected:

	virtual bool AddEventCallBack(const NFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb) = 0;
	virtual bool AddEventCallBack(const NFGUID self, const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb) = 0;

};

#endif