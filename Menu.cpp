//Interface plugin for Virtools Dev/3DVIA Virtools
//Virtools Script2Json & Building Block toolbox
//By Chris Xiong, 2018
//License: 3-Clause BSD
/*
Copyright (c) 2018, Chris Xiong
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the names of its
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h"
#include "Menu.h"
#include <map>
#include <utility>
#include <sstream>
#include <string>
#include "json/json.h"

extern PluginInterface*		s_Plugininterface;

void PluginMenuCallback(int commandID);

//static main menu
CMenu*							s_MainMenu			= NULL;

#define PLUGINMENU_MAXENTRIES	20	//put there the max entries your menu may use

//adds the menu to Virtools Dev main menu
void InitMenu()
{
	if (!s_Plugininterface)
		return;

	s_MainMenu		= s_Plugininterface->AddPluginMenu(STR_MAINMENUNAME,PLUGINMENU_MAXENTRIES,NULL,(VoidFunc1Param)PluginMenuCallback);
}

//removes the menu from Virtools Dev main menu
void RemoveMenu()
{
	if (!s_Plugininterface || !s_MainMenu)
		return;

	s_Plugininterface->RemovePluginMenu(s_MainMenu);
}

//up to user.
//(May be called on new composition notification for instance)
//Note that first commandID can be 0
//but last command ID must be lesser thanPLUGINMENU_MAXENTRIES
void UpdateMenu()
{
	s_Plugininterface->ClearPluginMenu(s_MainMenu);		//clear menu

	s_Plugininterface->AddPluginMenuItem(s_MainMenu,0,"Fsck /dev/sda1");//dump to json
	s_Plugininterface->AddPluginMenuItem(s_MainMenu,-1,NULL,TRUE);

	//note : sub menu must have independent command ids that must be >=0
	CMenu* sub0 = s_Plugininterface->AddPluginMenuItem(s_MainMenu,0,"Fsck /dev/sdb",FALSE,TRUE);//bb manipulation

	s_Plugininterface->AddPluginMenuItem(sub0,1,"Fsck /dev/sdb1");//modify bb proto
	s_Plugininterface->AddPluginMenuItem(sub0,2,"Fsck /dev/sdb2");//not implemented

	s_Plugininterface->AddPluginMenuItem(s_MainMenu,-1,NULL,TRUE);
	s_Plugininterface->AddPluginMenuItem(s_MainMenu,10,"Exit Fsck");

	s_Plugininterface->UpdatePluginMenu(s_MainMenu);	//update menu,always needed when you finished to update the menu
														//unless you want the menu not to have Virtools Dev main menu color scheme.
}

//bool clone_script(CKParameter* source_script,CKParameter* dest_script);
//bool clone_behavior(CKBehavior* src,CKBehavior* dest,CKBehavior* destpar);
bool dump_script(CKParameter* script);
bool dump_behavior(CKBehavior* src,Json::Value* dest,Json::Value* destpar);
bool CKParameter2Json(CKParameter* p,Json::Value &v);
void modify_bb_guid();

//fill with your command IDs and your actions
void PluginMenuCallback(int commandID)
{
	switch(commandID)
	{
		case 0:
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			CKContext* ctx=s_Plugininterface->GetCKContext();
			ParamEditDlg shitdialog(ctx);
			shitdialog.SetTitle("Pick the shit you want to materialize");
			CKParameter* srcshit=(CKParameter*)ctx->CreateObject(CKCID_PARAMETER,"source shit");
			//CKParameter* destshit=(CKParameter*)ctx->CreateObject(CKCID_PARAMETER,"dest shit");
			srcshit->SetGUID(CKPGUID_SCRIPT);
			//destshit->SetGUID(CKPGUID_SCRIPT);
			shitdialog.SetParameter(srcshit);
			shitdialog.DoModal();
			//shitdialog.SetParameter(destshit);
			//shitdialog.DoModal();
			//clone_script(srcshit,destshit);
			dump_script(srcshit);
			ctx->DestroyObject(srcshit);
			//ctx->DestroyObject(destshit);
		}
		break;
		case 1:
			modify_bb_guid();
		break;
		case 10:
			s_Plugininterface->GetCKContext()->OutputToConsole(
				"Fsck for Virtools 0.01\n"
				"A FileSystem Consistency Korruption tool.\n"
				"By Chris Xiong, 2018\n"
				"No warranty of any fscking kind comes with this fsck.\n\n"
				"Copyright (c) 2018, Chris Xiong\n"
				"All rights reserved.\n"
				"\n"
				"Redistribution and use in source and binary forms, with or without\n"
				"modification, are permitted provided that the following conditions are met:\n"
				"    * Redistributions of source code must retain the above copyright\n"
				"      notice, this list of conditions and the following disclaimer.\n"
				"    * Redistributions in binary form must reproduce the above copyright\n"
				"      notice, this list of conditions and the following disclaimer in the\n"
				"      documentation and/or other materials provided with the distribution.\n"
				"    * Neither the name of the copyright holder nor the names of its\n"
				"      contributors may be used to endorse or promote products derived from\n"
				"      this software without specific prior written permission.\n"
				"\n"
				"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n"
				"ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
				"WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
				"DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n"
				"FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
				"DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
				"SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED\n"
				"AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
				"(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
				"SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
			);
		break;
	}
}

template<typename Tag,typename Tag::type M>
struct rob
{ 
	friend typename Tag::type get(Tag)
	{return M;}
};
struct Behavior_blockdata
{
	typedef BehaviorBlockData* CKBehavior::*type;
	friend type get(Behavior_blockdata);
};
template struct rob<Behavior_blockdata,&CKBehavior::m_BlockData>;
void modify_bb_guid()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CKContext* ctx=s_Plugininterface->GetCKContext();
	MultiParamEditDlg shitdialog(ctx);
	CKParameter* bb=(CKParameter*)ctx->CreateObject(CKCID_PARAMETER,"Building block");
	CKParameter* g1=(CKParameter*)ctx->CreateObject(CKCID_PARAMETER,"GUID upper part (as int32_t)");
	CKParameter* g2=(CKParameter*)ctx->CreateObject(CKCID_PARAMETER,"GUID lower part (as int32_t)");
	bb->SetGUID(CKPGUID_BEHAVIOR);
	g1->SetGUID(CKPGUID_INT);
	g2->SetGUID(CKPGUID_INT);
	shitdialog.AddParameter(bb);
	shitdialog.AddParameter(g1);
	shitdialog.AddParameter(g2);
	shitdialog.SetTitle("Fscking a building block up");
	shitdialog.DoModal();
	CKBehavior* bbr=(CKBehavior*)bb->GetValueObject();
	/*Some stupid reverse engineering goes on here
	CKFile* f=bb->GetCKContext()->CreateCKFile();
	bbr->PreSave(f,0);
	CKStateChunk* c=bbr->Save(f,0);
	int dl=c->ConvertToBuffer(NULL);
	char* dump=new char[dl+1];
	c->ConvertToBuffer((void*)dump);
	if(bbr->GetPrototypeGuid().d1&&bbr->GetPrototypeGuid().d2)
	{//looks for proto GUID
		CKGUID og=bbr->GetPrototypeGuid();
		for(int i=0;i<dl-8;++i)
		{
			if(CKGUID(*reinterpret_cast<DWORD*>(dump+i),*reinterpret_cast<DWORD*>(dump+i+4))==og)
			ctx->OutputToConsoleEx("found proto GUID at offset %d",i);
		}
	}
	if(bbr->GetOwner())
	{//looks for owner
		CK_ID ow=bbr->GetOwner()->GetID();
		for(int i=0;i<dl-4;++i)
		{
			if(*reinterpret_cast<DWORD*>(dump+i)==ow)
			ctx->OutputToConsoleEx("found owner id at offset %d",i);
		}
	}
	if(bbr->GetParent())
	{//looks for parent behavior
		CK_ID par=bbr->GetParent()->GetID();
		for(int i=0;i<dl-4;++i)
		{
			if(*reinterpret_cast<DWORD*>(dump+i)==par)
			ctx->OutputToConsoleEx("found parent behavior id at offset %d",i);
		}
	}
	delete[] dump;*/
	BehaviorBlockData* bbd=bbr->*get(Behavior_blockdata());
	/*char dump[128];memcpy(dump,bbd,sizeof(dump));
	CKGUID og=bbr->GetPrototypeGuid();int f=-10000000;
	for(int i=0;i<31;++i){
		if(CKGUID(dump[i],dump[i+1])==og){f=i;break;}
		if(CKGUID(dump[i+1],dump[i])==og){f=-i-1;break;}
	}
	ctx->OutputToConsoleEx(f>-10000000?"found at offset %d!":"not found",f);*/
	CKGUID g;memcpy(&g.d1,g1->GetReadDataPtr(),sizeof(g.d1));
	memcpy(&g.d2,g2->GetReadDataPtr(),sizeof(g.d2));
	if(bbd)memcpy(bbd,&g.d,sizeof(&g.d));
	bbr->NotifyEdition();//does not work... shit
	ctx->OutputToConsole("Copy and paste the selected behavior to generate modified building blocks!");
	ctx->DestroyObject(bb);
	ctx->DestroyObject(g1);
	ctx->DestroyObject(g2);
}

//static CKBeObject *destowner=NULL;

/*bool clone_script(CKParameter* source_script,CKParameter* dest_script)
{
	CKContext* ctx=source_script->GetCKContext();
	CKBehavior* src=(CKBehavior*)source_script->GetValueObject();
	CKBehavior* dest=(CKBehavior*)dest_script->GetValueObject();
	if(!src||!dest)return false;
	destowner=NULL;
	clone_behavior(src,dest,NULL);
	return false;
}*/
bool dump_script(CKParameter* script)
{
	Json::Value ret;
	CKContext* ctx=script->GetCKContext();
	CKBehavior* src=(CKBehavior*)script->GetValueObject();
	if(!src)return false;
	dump_behavior(src,&ret,NULL);
	Json::StreamWriterBuilder w;
	std::ostringstream os;
	Json::StreamWriter* sw=w.newStreamWriter();
	sw->write(ret,&os);
	std::string stret=os.str();
	char* ck2isstupid=new char[stret.length()+1];
	ck2isstupid[stret.length()]=0;
	memcpy(ck2isstupid,stret.c_str(),stret.length());
	char strFilter[]={"JSON (*.json)|*.json|"};
	CFileDialog FileDlg(FALSE,CString(".json"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,CString(strFilter));
	FileDlg.m_ofn.lpstrTitle="Save the fucking JSON";
	FileDlg.DoModal();
	CString p=FileDlg.GetFolderPath()+TEXT("\\")+FileDlg.GetFileName();
	CFile f;f.Open(p.GetString(),CFile::modeCreate|CFile::modeReadWrite);
	f.Write(ck2isstupid,stret.length());f.Close();
	ctx->OutputToConsole("Done!",true);
	OutputDebugStringA(ck2isstupid);
	delete[] ck2isstupid;
	delete sw;
	return true;
}

const char* beht2str(CK_BEHAVIOR_TYPE t)
{
	switch(t)
	{
		case CKBEHAVIORTYPE_BASE:return "block";
		case CKBEHAVIORTYPE_BEHAVIOR:return "graph";
		case CKBEHAVIORTYPE_SCRIPT:return "script";
	}
	return "shit";
}

void printdumb(const char* fmt,...)
{
	char s[1024];
	va_list args;
	va_start(args,fmt);
	vsnprintf(s,1024,fmt,args);
	OutputDebugStringA(s);
	va_end(args);
}

#define rienabranler(a,b) (a?a->b()?a->b():"(noname)":"--NULL--")
#define rienabranleri(a,b) (a?a->b()?a->b():-1:-1)
void dump_parameter_in(CKParameterIn* pin,Json::Value& v)
{
	v["id"]=Json::Value(pin->GetID());
	v["name"]=Json::Value(pin->GetName());
	v["type"]=Json::Value(pin->GetCKContext()->GetParameterManager()->ParameterGuidToName(pin->GetGUID()));
	v["type_guid"]=Json::Value();
	v["type_guid"][0]=Json::Value(pin->GetGUID().d1);
	v["type_guid"][1]=Json::Value(pin->GetGUID().d2);
	v["real_source_id"]=Json::Value(rienabranleri(pin->GetRealSource(),GetID));
	v["direct_source_id"]=Json::Value(rienabranleri(pin->GetDirectSource(),GetID));
	v["shared_source_id"]=Json::Value(rienabranleri(pin->GetSharedSource(),GetID));
}
void dump_parameter_out(CKParameterOut* pout,Json::Value& v)
{
	v["id"]=Json::Value(pout->GetID());
	v["name"]=Json::Value(pout->GetName());
	v["type"]=Json::Value(pout->GetCKContext()->GetParameterManager()->ParameterGuidToName(pout->GetGUID()));
	v["type_guid"]=Json::Value();
	v["type_guid"][0]=Json::Value(pout->GetGUID().d1);
	v["type_guid"][1]=Json::Value(pout->GetGUID().d2);
	v["dest"]=Json::Value();
	for(int i=0,c=pout->GetDestinationCount();i<c;++i)
		v["dest"][i]=pout->GetDestination(i)->GetID();
}
bool dump_behavior(CKBehavior* src,Json::Value* dest,Json::Value* destpar)
{
	printdumb("type: %s, sub count: %d, name: %s, proto: %s\n",beht2str(src->GetType()),
		src->GetSubBehaviorCount(),
		src->GetName(),
		src->GetPrototypeName()?src->GetPrototypeName():"null"
	);
	Json::Value& dst=*dest;
	dst["type"]=Json::Value(src->GetType());
	if(src->GetType()==CKBEHAVIORTYPE_BASE)
	{
		dst["proto_guid"]=Json::Value();
		dst["proto_guid"][0]=Json::Value(src->GetPrototypeGuid().d1);
		dst["proto_guid"][1]=Json::Value(src->GetPrototypeGuid().d2);
	}
	dst["name"]=Json::Value(std::string(src->GetName()));
	dst["flags"]=Json::Value(Json::UInt(src->GetFlags()));
	dst["priority"]=Json::Value(src->GetPriority());
	dst["version"]=Json::Value(src->GetVersion());
	if(src->IsUsingTarget())
	{
		dst["target"]=src->GetTargetParameter()->GetRealSource()->GetID();
	}
	if(std::string(src->GetName())==std::string("Op")&&src->GetLocalParameterCount()>2)
	{
		CKGUID g;memcpy(&g.d1,src->GetLocalParameter(1)->GetReadDataPtr(false),sizeof(g.d1));
		memcpy(&g.d2,src->GetLocalParameter(2)->GetReadDataPtr(false),sizeof(g.d2));
		dst["op_type"]=Json::Value(std::string(src->GetCKContext()->GetParameterManager()->
		OperationGuidToName(g)));
	}
	if(src->GetInputCount())
	{
		dst["inputs"]=Json::Value();
		for(int i=0,c=src->GetInputCount();i<c;++i)
		{
			Json::Value &v=dst["inputs"][i]=Json::Value();
			v["name"]=Json::Value(std::string(src->GetInput(i)->GetName()));
			v["id"]=Json::Value(src->GetInput(i)->GetID());
		}
	}
	if(src->GetOutputCount())
	{
		dst["outputs"]=Json::Value();
		for(int i=0,c=src->GetOutputCount();i<c;++i)
		{
			Json::Value &v=dst["outputs"][i]=Json::Value();
			v["name"]=Json::Value(std::string(src->GetOutput(i)->GetName()));
			v["id"]=Json::Value(src->GetOutput(i)->GetID());
		}
	}
	if(src->GetSubBehaviorCount())
	{
		dst["subbehavior"]=Json::Value();
		for(int i=0,c=src->GetSubBehaviorCount();i<c;++i)
		{
			dst["subbehavior"][i]=Json::Value();
			dump_behavior(src->GetSubBehavior(i),&dst["subbehavior"][i],NULL);
		}
	}
	if(src->GetSubBehaviorLinkCount())
	{
		dst["behavior_link"]=Json::Value();
		for(int i=0,c=src->GetSubBehaviorLinkCount();i<c;++i)
		{
			Json::Value &v=dst["behavior_link"][i]=Json::Value();
			v["in"]=Json::Value(src->GetSubBehaviorLink(i)->GetInBehaviorIO()->GetID());
			v["out"]=Json::Value(src->GetSubBehaviorLink(i)->GetOutBehaviorIO()->GetID());
			v["delay"]=Json::Value(src->GetSubBehaviorLink(i)->GetInitialActivationDelay());
		}
	}
	if(src->GetLocalParameterCount())
	{
		dst["local_parameter"]=Json::Value();
		for(int i=0,c=src->GetLocalParameterCount();i<c;++i)
		{
			Json::Value &v=dst["local_parameter"][i]=Json::Value();
			v["id"]=Json::Value(src->GetLocalParameter(i)->GetID());
			v["name"]=Json::Value(rienabranler(src->GetLocalParameter(i),GetName));
			if(src->IsLocalParameterSetting(i))
			v["issetting"]=Json::Value(1);
			CKParameter2Json(src->GetLocalParameter(i),v);
		}
	}
	if(src->GetInputParameterCount())
	{
		dst["input_parameter"]=Json::Value();
		for(int i=0,c=src->GetInputParameterCount();i<c;++i)
		{
			Json::Value &v=dst["input_parameter"][i]=Json::Value();
			dump_parameter_in(src->GetInputParameter(i),v);
		}
	}
	if(src->GetOutputParameterCount())
	{
		dst["output_parameter"]=Json::Value();
		for(int i=0,c=src->GetOutputParameterCount();i<c;++i)
		{
			Json::Value &v=dst["output_parameter"][i]=Json::Value();
			dump_parameter_out(src->GetOutputParameter(i),v);
		}
	}
	if(src->GetParameterOperationCount())
	{
		dst["parameter_operation"]=Json::Value();
		for(int i=0,c=src->GetParameterOperationCount();i<c;++i)
		{
			Json::Value &v=dst["parameter_operation"][i]=Json::Value();
			v["op"]=Json::Value(std::string(src->GetCKContext()->GetParameterManager()->
				OperationGuidToName(src->GetParameterOperation(i)->GetOperationGuid())));
			CKGUID g=src->GetParameterOperation(i)->GetOperationGuid();
			char s[64];snprintf(s,64,"0x%x,0x%x",g.d1,g.d2);
			v["op_guid"]=Json::Value(s);
			v["p1"]=Json::Value();
			dump_parameter_in(src->GetParameterOperation(i)->GetInParameter1(),v["p1"]);
			dump_parameter_in(src->GetParameterOperation(i)->GetInParameter2(),v["p2"]);
			dump_parameter_out(src->GetParameterOperation(i)->GetOutParameter(),v);
		}
	}
	return true;
}

//typedef std::map<CKBehaviorIO*,std::pair<CKBehavior*,int>> shitmap;

/*bool clone_behavior(CKBehavior* src,CKBehavior* dest,CKBehavior* destpar)
//This is still incomplete (does not clone any parameters), yet it refuses to work.
{
	printdumb("type: %s, sub count: %d, proto: %s\n",beht2str(src->GetType()),
		src->GetSubBehaviorCount(),
		src->GetPrototypeName()?src->GetPrototypeName():"null"
	);
	//copy properties src -> dest
	shitmap sm;
	if(src->GetType()==CKBEHAVIORTYPE_BASE)
	{
		dest->InitFromPrototype(src->GetPrototype());
		dest->InitFctPtrFromPrototype(src->GetPrototype());
	}
	else dest->SetType(src->GetType());
	dest->SetFlags(src->GetFlags());
	dest->SetPriority(src->GetPriority());
	dest->SetVersion(src->GetVersion());
	//if(dest->GetOwner())destowner=dest->GetOwner();
	//else if(destowner)destowner->AddScript(dest);else printdumb("fuck\n");
	if(src->GetType()==CKBEHAVIORTYPE_BEHAVIOR)
	{
		for(int i=0;i<src->GetOutputCount();++i)
		{
			dest->AddOutput(src->GetOutput(i)->GetName());
			sm[src->GetOutput(i)]=std::make_pair(dest,-i-1);
		}
		for(int i=0;i<src->GetInputCount();++i)
		{
			dest->AddInput(src->GetInput(i)->GetName());
			sm[src->GetInput(i)]=std::make_pair(dest,i);
		}
		//also do this for parameters
	}
	if(src->GetType()==CKBEHAVIORTYPE_SCRIPT)
	{
		sm[src->GetInput(0)]=std::make_pair(dest,0);
	}
	if(destpar){CKERROR r=destpar->AddSubBehavior(dest);if(r!=CK_OK)printdumb("add sub behavior failed: %d - %s\n",r,CKErrorToString(r));}
	printdumb("src fnc: %p, dest fnc: %p\n",src->GetFunction(),dest->GetFunction());
	printdumb("src owner: %s, dest owner: %s\n",rienabranler(src->GetOwner(),GetName),rienabranler(dest->GetOwner(),GetName));
	printdumb("src target: %s, dest target: %s\n",rienabranler(src->GetTarget(),GetName),rienabranler(dest->GetTarget(),GetName));
	printdumb("src ver: %x, dest ver: %x\n",src->GetVersion(),dest->GetVersion());
	printdumb("src root: %s, dest root: %s\n",rienabranler(src->GetOwnerScript(),GetName),rienabranler(dest->GetOwnerScript(),GetName));
	printdumb("==============================================\n");
	int c=src->GetSubBehaviorCount();
	for(int i=0;i<c;++i)
	{
		CKBehavior* cur=src->GetSubBehavior(i);
		//create dest for current shit
		CKBehavior* curdest=(CKBehavior*)src->GetCKContext()->CreateObject(CKCID_BEHAVIOR,cur->GetName());
		int ic=cur->GetInputCount(),oc=cur->GetOutputCount();
		for(int j=0;j<ic;++j)
		sm[cur->GetInput(j)]=std::make_pair(curdest,j);
		for(int j=0;j<oc;++j)
		sm[cur->GetOutput(j)]=std::make_pair(curdest,-j-1);
		clone_behavior(cur,curdest,dest);
	}
	c=src->GetSubBehaviorLinkCount();
	for(int i=0;i<c;++i)
	{
		CKBehaviorLink* cur=src->GetSubBehaviorLink(i);
		CKBehaviorLink* dst=(CKBehaviorLink*)src->GetCKContext()->CreateObject(CKCID_BEHAVIORLINK,"");
		dst->SetInitialActivationDelay(cur->GetInitialActivationDelay());
		dst->SetActivationDelay(cur->GetActivationDelay());
		if(sm.find(cur->GetInBehaviorIO())==sm.end()||sm.find(cur->GetOutBehaviorIO())==sm.end())
		{
			printdumb("fuck shit! at least one of these behavior io is not found in the map:"
			" %s:%s %s:%s\n",cur->GetInBehaviorIO()->GetOwner()->GetName(),cur->GetInBehaviorIO()->GetName(),
			cur->GetOutBehaviorIO()->GetOwner()->GetName(),cur->GetOutBehaviorIO()->GetName());
			continue;
		}
		dst->SetInBehaviorIO(
			sm[cur->GetInBehaviorIO()].second<0?
			sm[cur->GetInBehaviorIO()].first->GetOutput(-sm[cur->GetInBehaviorIO()].second-1):
			sm[cur->GetInBehaviorIO()].first->GetInput(sm[cur->GetInBehaviorIO()].second)
		);
		dst->SetOutBehaviorIO(
			sm[cur->GetOutBehaviorIO()].second<0?
			sm[cur->GetOutBehaviorIO()].first->GetOutput(-sm[cur->GetOutBehaviorIO()].second-1):
			sm[cur->GetOutBehaviorIO()].first->GetInput(sm[cur->GetOutBehaviorIO()].second)
		);
	}
	return true;
}*/

bool CKParameter2Json(CKParameter* p,Json::Value &v)
{
	CKGUID t=p->GetGUID();bool u=0;
	if(t.d1&t.d2)
	v["type"]=Json::Value(std::string(p->GetCKContext()->GetParameterManager()->ParameterGuidToName(t)));
	else v["type"]=Json::Value("unknown");
	if(v["type"]==Json::Value("unknown"))
	{
		char s[64];snprintf(s,64,"unknown: 0x%x, 0x%x",p->GetGUID().d1,p->GetGUID().d2);
		v["type"]=Json::Value(s);
		u=1;
	}
	//nothing
	if(t==CKPGUID_NONE)
	{return true;}
	if(p->GetParameterClassID()&&p->GetValueObject(false))
	{
		v["data"]=Json::Value();
		v["data"]["id"]=Json::Value(p->GetValueObject(false)->GetID());
		v["data"]["name"]=p->GetValueObject(false)->GetName();
		v["data"]["type"]=p->GetValueObject(false)->GetClassNameA();
		return true;
	}
	//float
	if(t==CKPGUID_FLOAT||t==CKPGUID_ANGLE||t==CKPGUID_PERCENTAGE||t==CKPGUID_TIME
	||t==CKPGUID_FLOATSLIDER)
	{
		v["data"]=Json::Value(*(float*)(p->GetReadDataPtr(false)));
		return true;
	}
	//int
	if(t==CKPGUID_INT||t==CKPGUID_KEY||t==CKPGUID_BOOL||t==CKPGUID_ID||t==CKPGUID_POINTER
	||t==CKPGUID_MESSAGE||t==CKPGUID_ATTRIBUTE||t==CKPGUID_BLENDMODE||t==CKPGUID_FILTERMODE
	||t==CKPGUID_BLENDFACTOR||t==CKPGUID_FILLMODE||t==CKPGUID_LITMODE||t==CKPGUID_SHADEMODE
	||t==CKPGUID_ADDRESSMODE||t==CKPGUID_WRAPMODE||t==CKPGUID_3DSPRITEMODE||t==CKPGUID_FOGMODE
	||t==CKPGUID_LIGHTTYPE||t==CKPGUID_SPRITEALIGN||t==CKPGUID_DIRECTION||t==CKPGUID_LAYERTYPE
	||t==CKPGUID_COMPOPERATOR||t==CKPGUID_BINARYOPERATOR||t==CKPGUID_SETOPERATOR
	||t==CKPGUID_OBSTACLEPRECISION||t==CKPGUID_OBSTACLEPRECISIONBEH)
	{
		v["data"]=Json::Value(*(int*)(p->GetReadDataPtr(false)));
		return true;
	}
	if(t==CKPGUID_VECTOR)
	{
		VxVector vec;
		memcpy(&vec,p->GetReadDataPtr(false),sizeof(vec));
		v["data"]=Json::Value();v["data"]["x"]=vec.x;
		v["data"]["y"]=vec.y;v["data"]["z"]=vec.z;
		return true;
	}
	if(t==CKPGUID_2DVECTOR)
	{
		Vx2DVector vec;
		memcpy(&vec,p->GetReadDataPtr(false),sizeof(vec));
		v["data"]=Json::Value();v["data"]["x"]=vec.x;
		v["data"]["y"]=vec.y;
		return true;
	}
	if(t==CKPGUID_MATRIX)
	{
		VxMatrix mat;
		memcpy(&mat,p->GetReadDataPtr(false),sizeof(mat));
		v["data"]=Json::Value();for(int i=0;i<4;++i)v["data"][i]=Json::Value();
		for(int i=0;i<4;++i)for(int j=0;j<4;++j)v["data"][i][j]=Json::Value(mat[i][j]);
		return true;
	}
	if(t==CKPGUID_COLOR)
	{
		VxColor col;
		memcpy(&col,p->GetReadDataPtr(false),sizeof(col));
		v["data"]=Json::Value();v["data"]["r"]=col.r;
		v["data"]["g"]=col.g;v["data"]["b"]=col.b;
		v["data"]["a"]=col.a;
		return true;
	}
	if(t==CKPGUID_2DCURVE)
	{
		Json::Value &vv=v["data"]=Json::Value();
		CK2dCurve *c;memcpy(&c,p->GetReadDataPtr(false),sizeof(c));
		for(int i=0,cc=c->GetControlPointCount();i<cc;++i)
		{
			vv[i]=Json::Value();
			vv[i]["pos"]=Json::Value();
			vv[i]["pos"]["x"]=Json::Value(c->GetControlPoint(i)->GetPosition().x);
			vv[i]["pos"]["y"]=Json::Value(c->GetControlPoint(i)->GetPosition().y);
			vv[i]["islinear"]=Json::Value(c->GetControlPoint(i)->IsLinear());
			if(c->GetControlPoint(i)->IsTCB())
			{
				vv[i]["bias"]=Json::Value(c->GetControlPoint(i)->GetBias());
				vv[i]["continuity"]=Json::Value(c->GetControlPoint(i)->GetContinuity());
				vv[i]["tension"]=Json::Value(c->GetControlPoint(i)->GetTension());
			}
			else
			{
				vv[i]["intangent"]=Json::Value();
				vv[i]["intangent"]["x"]=Json::Value(c->GetControlPoint(i)->GetInTangent().x);
				vv[i]["intangent"]["y"]=Json::Value(c->GetControlPoint(i)->GetInTangent().y);
				vv[i]["outtangent"]=Json::Value();
				vv[i]["outtangent"]["x"]=Json::Value(c->GetControlPoint(i)->GetOutTangent().x);
				vv[i]["outtangent"]["y"]=Json::Value(c->GetControlPoint(i)->GetOutTangent().y);
			}
		}
	}
	u=1;//if it gets here, we have no idea what it really is. so simply dump it.
	//buffer-like
	if(u||t==CKPGUID_VOIDBUF||t==CKPGUID_STRING||t==CKPGUID_SHADER||t==CKPGUID_TECHNIQUE||t==CKPGUID_PASS)
	{
		std::string d((char*)p->GetReadDataPtr(false),p->GetDataSize());
		v["data"]=Json::Value(d);
		v["length"]=Json::Value(p->GetDataSize());
		return true;
	}
	return false;
}