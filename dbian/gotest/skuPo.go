package main

import "database/sql"

//结构名大写才能被外文件访问，小写都是对内
type SourceSku struct {
	Fskuid int64
	Fcooperatorsubaccountid int64
	Fcooperatorid int32
	Fcategoryid int32
	Fskutype int32
	Fskuoperationmodel int32
	Fbarcode string
	Fskulocalcode string
	Fskudimensionalbarcode string
	Fskutitle string
	Fskusubtitle string
	Fskupromotdesc string
	Fskushortname string
	Fskusaleattr string
	Fskusaleattrdesc string
	Fskukeyattr string
	Fskukeyattrdesc string
	Fskureferprice int64
	Fskuproperty int64
	Fskuproperty1 int64
	Fskustate int32
	Fskuweight int64
	Fskunetweight int64
	Fskuvolume int64
	Fskusizex int32
	Fskusizey int32
	Fskusizez int32
	Fskucategoryattr string
	Fskukeyword string
	Fskuvatrate int32
	Fskupurchtaxrate int32
	Fskusnapversion int32
	Fskubuylimit int32
	Fskubrand int32
	Fskuexchangepoint int32
	Fmainskuid int64
	Fskulastuptime int64
	Fskulastdowntime int64
	Fskuaddtime int64
	Fskulastupdatetime int64
	Flastupdatetime int64
	Fskudurability int32
	Fskutag string
	Fskusort int32
	Fskuexpertscomments string
	Fskupurchprice int32
	Fskuunit string
	Fpopskucode string
	Fskueffectivetime int64
	Fskuexpiretime int64
	Fskutwodimcodelastprinttime int64
	Fskutwodimcodelastprintperson string
	Fskustockexpiretime int64
	Fskunameindex string
	Fskuspecmodel string
	Fskuproducingarea string
	Fskupackrate int64
	Fskupacklist string
	Fskuceilprice int64
	Fskufloorprice int32
	Fskuunitcost int32
	Fskucontacternum int32
	Fskutravellernum int32
	Fskusalearea string
	Fplatformid int32
	Fcontractid string
	Fflagshipstoreid int32
	Fmainurl string
	Fspuid int64
	Fcustomserviceid int32
	Fcertifyurl string
	Fcertifyurlid string
	Fadpicurl string
	Fadlink string
	Fadbegintime int64
	Fadendtime int64
	Fskuservicenum int32
	Fskuappage string
	Fsexid int32
	Fisforce int32
	Fcurrency string
	Fsource int32
	Ftoptempid int64
	Fbottomtempid int64
	Ftempid int64
	Fsocialsofttxt string
}


const SelectSkuField = "Fskuid,Fcooperatorsubaccountid,Fskulocalcode,Fcooperatorid,Fcategoryid,Fskutype,Fskuoperationmodel, " +
"Fbarcode,Fskudimensionalbarcode,Fskutitle,Fskusubtitle,Fskupromotdesc,Fskusaleattr,Fskusaleattrdesc,Fskukeyattr, " +
"Fskukeyattrdesc,Fskureferprice,Fskupurchprice,Fskuproperty,Fskuproperty1,Fskustate,Fskuweight,Fskunetweight,Fskuvolume," +
"Fskusizex,Fskusizey,Fskusizez,Fskucategoryattr,Fskukeyword,Fskuvatrate,Fskusnapversion,Fskubuylimit,Fskubrand,Fskuexchangepoint," +
"Fmainskuid,UNIX_TIMESTAMP(Fskulastuptime) as skulastuptime,UNIX_TIMESTAMP(Fskulastdowntime) as skulastdowntime,UNIX_TIMESTAMP(Fskuaddtime) as skuaddtime," +
"UNIX_TIMESTAMP(Fskulastupdatetime) as skulastupdatetime,UNIX_TIMESTAMP(Flastupdatetime) as lastupdatetime,Fskupurchtaxrate,Fskushortname,Fskudurability," +
"Fskutag,Fskusort,Fskuexpertscomments,Fskuunit,Fpopskucode,UNIX_TIMESTAMP(Fskueffectivetime) as skueffectivetime,UNIX_TIMESTAMP(Fskuexpiretime) as skuexpiretime," +
"UNIX_TIMESTAMP(Fskutwodimcodelastprinttime) as skutwodimcodelastprinttime,Fskutwodimcodelastprintperson,UNIX_TIMESTAMP(Fskustockexpiretime) as skustockexpiretime," +
"Fskuspecmodel,Fskuproducingarea,Fskupackrate,Fskupacklist,Fskuceilprice,Fskufloorprice,Fskuunitcost,Fskucontacternum,Fskutravellernum,Fskusalearea," +
"Fplatformid,Fcontractid,Fflagshipstoreid,Fspuid,Fcustomserviceid,Fcertifyurl,Fcertifyurlid,Fadpicurl,Fadlink,UNIX_TIMESTAMP(Fadbegintime) as adbegintime," +
"UNIX_TIMESTAMP(Fadendtime) as adendtime,Fskuservicenum,Fskuappage,Fsexid,Fisforce,Fcurrency,Fsource,Ftoptempid,Fbottomtempid,Ftempid,Fmainurl,Fsocialsofttxt," +
"Fskunameindex "

type SourceSpu struct {
	Fspuid int64
	Fcooperatorid int64
	Fcategoryid int32
	Foperationmodel int32
	Fgoodsno string
	Fsputype int32
	Fsputitle string
	Fspupromotdesc string
	Fspupromotdescefftime int64
	Fspupromotdescexptime int64
	Fspubrand int32
	Fspuspecmodel string
	Fspureferprice int32
	Fspuprice int32
	Fspuproperty int64
	Fspuproperty1 int64
	Fspucategoryattr string
	Fspuweight int32
	Fspusizex int32
	Fspusizey int32
	Fspusizez int32
	Fspuunit string
	Fspupackrate int32
	Fspupacklist string
	Fspustate int32
	Foperatetime int64
	Freason string
	Faddtime int64
	Flastupdatetime int64
	Fspuappage string
	Fsexid int32
	Ftempid int64
	Fisforce int32
	Ftoptempid int64
	Fbottomtempid int64
	Fsocialsofttxt string
}

const SelectSpuField = "Fspuid, Fcategoryid, Foperationmodel, Fgoodsno, Fsputype, Fsputitle, Fspupromotdesc, UNIX_TIMESTAMP(Fspupromotdescefftime) as promotdesceffectivetime, " +
	"UNIX_TIMESTAMP(Fspupromotdescexptime) as promotdescexpiretime, Fspubrand, Fspuspecmodel, Fspureferprice, Fspuprice, Fspuproperty, Fspuproperty1, Fspucategoryattr, " +
	"Fspuweight, Fspusizex, Fspusizey, Fspusizez, Fspuunit, Fspupackrate, Fspupacklist, Fspustate, UNIX_TIMESTAMP(Foperatetime) as operatetime, Freason, UNIX_TIMESTAMP(Faddtime) as spuaddtime, " +
	"UNIX_TIMESTAMP(Flastupdatetime) as spulastupdatetime, Fcooperatorid , Fspuappage, Fsexid , Ftempid , Ftempid , Ftoptempid , Fbottomtempid , Fisforce , Fsocialsofttxt"

type SoureArea struct {
	Fskuid	int64
	Fcooperatorsubaccountid	int64
	Fcooperatorskuareacode string
	Fcooperatorbarcode string
	Fpromotiontype int32
	Fskuareapromotdesc string
	Fskuareaprice int64
	Fskuareapreprice int64
	Fskuareacostprice int64
	Fskuareabusinesscost int32
	Fskuareaestimatedispatch string
	Fskuareaproperty int64
	Fskuareastate int32
	Fskuareaminbuycount int32
	Fskuareamaxbuycount int32
	Fskuareaaddtime int64
	Fskuarealastupdatetime int64
	Fskuareabuymultiple int32
	Fskuareastorelogistictype int32
	Fskuareafirstpublishtime int64
	Fskuareapromotdescefftime int64
	Fskuareapromotdescexptime int64
	Fskuareabuyer string
	Fskuareasharecommission int32
	Fentityid int32
	Fstoreid string
	Fchannelid int32
	Fdetail string
	Fworkstatecode string
	Fcirculationmodecode string
	Fskuareapostprice int64
	Fincatalog int32
	Fareasort int32
	Fsaleslabelid sql.NullString
	Fsaleslabelname string
	Fsaleslabelurl string
	Fsalesstarttime int64
	Fsalesendtime int64
	Fstorestate int32
	Fsharecommissionbegintime int64
	Fsharecommissionendtime int64
	Fcooperatorid int64
	Fisinstall int32
	Finstallcost int32
	Fcontractid string
	//nullAge sql.NullInt64
}

const SelectAreaField = "Fskuid,Fcooperatorsubaccountid,Fcooperatorskuareacode,Fcooperatorbarcode,Fpromotiontype,Fskuareapromotdesc,Fskuareaprice," +
	"Fskuareapreprice,Fskuareacostprice,Fskuareabusinesscost,Fskuareaestimatedispatch,Fskuareaproperty,Fskuareastate,Fskuareaminbuycount,Fskuareamaxbuycount,UNIX_TIMESTAMP(Fskuareaaddtime)," +
	"UNIX_TIMESTAMP(Fskuarealastupdatetime),Fskuareabuymultiple,Fskuareastorelogistictype,UNIX_TIMESTAMP(Fskuareafirstpublishtime),UNIX_TIMESTAMP(Fskuareapromotdescefftime)," +
	"UNIX_TIMESTAMP(Fskuareapromotdescexptime),Fskuareabuyer,Fskuareasharecommission, Fentityid,Fstoreid,Fchannelid,Fdetail,Fworkstatecode,Fcirculationmodecode,Fskuareapostprice,Fincatalog," +
	"Fareasort,Fsaleslabelid,Fsaleslabelname,Fsaleslabelurl,UNIX_TIMESTAMP(Fsalesstarttime),UNIX_TIMESTAMP(Fsalesendtime)," +
	"Fstorestate,UNIX_TIMESTAMP(Fsharecommissionbegintime),UNIX_TIMESTAMP(Fsharecommissionendtime),Fcooperatorid,Fisinstall,Finstallcost,Fcontractid"

type NewSku struct {
	Fplatformid	int32
	Fspuid string
	Fskuid int64
	Fskuextcode string
	Fskulocalcode string
	Fgoodsno string
	Fskustate int32
	Fcategoryattr string
	Fskusaleattr string
	Fskukeyattr string
	Fskupacklist string
	Fskumainurl string
	Fskuminbuycount int32
	Fskubuymultiple int32
	Fskumaxbuycount int32
	Fskuunitcost int32
	Fskuerpreferprice int64
	Fskuerppurchprice int64
	Fskuerpceilingprice int64
	Fskuerpfloorprice int32
	Fgrossmargin int32
	Fskuproperty int64
	Fskuunit string
	Fskulevel string
	Fskustockexpiretime int64
	Fskuaddtime int64
	Fskulastupdatetime int64
	Fskulastupstatetime int64
	Fskulastdowntime int64
	Fskufirstpublishtime int64
}

const NewSkuField = "Fplatformid,Fspuid ,Fskuid ,Fskuextcode,Fskulocalcode,Fgoodsno ,Fskustate,Fcategoryattr,Fskusaleattr ,Fskukeyattr,Fskupacklist ," +
	"Fskumainurl,Fskuminbuycount,Fskubuymultiple,Fskumaxbuycount,Fskuunitcost ,Fskuerpreferprice,Fskuerppurchprice,Fskuerpceilingprice,Fskuerpfloorprice," +
	"Fgrossmargin ,Fskuproperty ,Fskuunit ,Fskulevel,UNIX_TIMESTAMP(Fskustockexpiretime),UNIX_TIMESTAMP(Fskuaddtime),UNIX_TIMESTAMP(Fskulastupdatetime) ," +
	"UNIX_TIMESTAMP(Fskulastupstatetime),UNIX_TIMESTAMP(Fskulastdowntime) ,UNIX_TIMESTAMP(Fskufirstpublishtime)"

type NewSpu struct {
	Fplatformid int32
	Fspuid string
	Fspuextcode string
	Fmainspuid string
	Fcooperatorid int64
	Fflagshipstoreid int32
	Fsputitle string
	Fspuonlinetitle string
	Fspushortname string
	Fspusubtitle string
	Fspustate int32
	Fspuoperationmodel int32
	Fsputype int32
	Fspumainurl string
	Fbrandid int32
	Ferpbrandid string
	Fcategoryid int32
	Ferppubnavid string
	Fspusaleattr string
	Fspucategoryattr string
	Ftempid int64
	Ftoptempid int64
	Fbottomtempid int64
	Fspuproperty int64
	Fspuvatrate int32
	Fspupurchtaxrate int32
	Fspuproducingarea string
	Fspuspecmodel string
	Fspusort int32
	Fsaleslabelid sql.NullString
	Fmcid int64
	Fremark string
	Fcreateuser string
	Fspuaddtime int64
	Fspulastupdatetime int64
	Fspulastupstoretime int64
	Fspulastdownstoretime int64
}

const NewSpuField = "Fplatformid,Fspuid,Fspuextcode,Fmainspuid,Fcooperatorid,Fflagshipstoreid,Fsputitle,Fspuonlinetitle,Fspushortname,Fspusubtitle,Fspustate," +
	"Fspuoperationmodel,Fsputype,Fspumainurl,Fbrandid,Ferpbrandid,Fcategoryid,Ferppubnavid,Fspusaleattr,Fspucategoryattr,Ftempid,Ftoptempid,Fbottomtempid," +
	"Fspuproperty,Fspuvatrate,Fspupurchtaxrate,Fspuproducingarea,Fspuspecmodel,Fspusort,Fsaleslabelid,Fmcid,Fremark,Fcreateuser," +
	"UNIX_TIMESTAMP(Fspuaddtime),UNIX_TIMESTAMP(Fspulastupdatetime),UNIX_TIMESTAMP(Fspulastupstoretime),UNIX_TIMESTAMP(Fspulastdownstoretime)"

type NewArea struct {
	Fplatformid int32
	Fspuid string
	Fskuid int64
	Fentityid int32
	Fchannelid int32
	Fstoreid string
	Fincatalog int32
	Fworkstatecode string
	Fcirculationmodecode string
	Fskuareapostprice int64
	Fskuareaprice int64
	Fskuareapreprice int64
	Fsaleslabelid sql.NullString
	Fsubchannelinfo string
	Fskuareaproperty int64
 	Fskuareabuyer string
	Fskuareaaddtime int64
	Fskuarealastupdatetime int64
	Fskuareafirstpublishtime int64
}

const NewAreaField = "Fplatformid,Fspuid,Fskuid,Fentityid,Fchannelid,Fstoreid,Fincatalog,Fworkstatecode,Fcirculationmodecode,Fskuareapostprice," +
	"Fskuareaprice,Fskuareapreprice,Fsaleslabelid,Fsubchannelinfo,Fskuareaproperty,Fskuareabuyer," +
	"UNIX_TIMESTAMP(Fskuareaaddtime),UNIX_TIMESTAMP(Fskuarealastupdatetime),UNIX_TIMESTAMP(Fskuareafirstpublishtime)"

type SkuErp struct {
	Fskuid int64
	Fgoodsno string
	Fgrossmargin int32
	Fskulevel string
	Fskureferprice int64
	Fskupurchpricev2 int64
	Fsputitle sql.NullString
	Fspuid int64
	Fbrandid string
	Fcategoryid string
}

const SkuErpField = "Fskuid,Fgoodsno,Fgrossmargin,Flevel,Fskureferprice,Fskupurchpricev2,Fskutitle,Fspuid,Fbrandid,Fcategoryid"

type CFilter struct {
	Fskuid int32
}

type EsReq struct {
	PlatformNum string `json:"platformNum"`
    AppCode string `json:"appCode"`
    Token string `json:"token"`
    Start int32 `json:"start"`
    Limit int32 `json:"limit"`
    Fls string `json:"fls"`
    Ids string `json:"ids"`
    Span string `json:"span"`
    SpanDynamic string `json:"spanDynamic"`
    Sort string `json:"-"`
    Query string `json:"-"`
    Filter CFilter `json:"_filter"`
	Group string `json:"-"`
	Highlight string `json:"-"`
}

type Content struct {
	Fspuid string
	Fbrandname string
	Fbrandid int32
	Ferpbrandname string
	Ferpbrandid string
}

type Resresult struct {
	List []Content `json:"list"`
	RealTotal int32 `json:"realTotal"`
}

type ResContent struct {
	Result Resresult `json:"result"`
}

type ResData struct {
	Success bool `json:"success"`
	Code string `json:"code"`
	Msg string `json:"msg"`
	Content ResContent `json:"content"`
}