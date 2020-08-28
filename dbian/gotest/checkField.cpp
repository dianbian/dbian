package main
/*
   create by bianbian, 2020/6/2
*/

import (
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"os"
	"strconv"
	"strings"
	"time"
	"flag"
)

const printlnlog bool  = true

var mapDomain map[int64]SourceSku
var mapSpu map[int64]SourceSpu
var mapArea map[string]SoureArea
var mapNewArea map[string]NewArea
var mapNewSku map[int64]NewSku
var mapSkuErp map[int64]SkuErp
var mapNewSpu map[string]NewSpu

//包还要排序

func selectSkuField(db *sql.DB, mapSku map[int64]SourceSku, skuidVec [2000] int64)  {
	var pinStr = ""
	//var selectStr = "select " + SelectSkuField + " from t_sku Order By Flastupdatetime Desc limit 100"
	var selectStr = "select " + SelectSkuField + " from t_sku"

	if skuidVec[0] != 0 {
		pinStr += "("
		for i := 0; i < len(skuidVec); i++ {
			if skuidVec[i] == 0 {
				continue
			}
			pinStr += strconv.FormatInt(skuidVec[i], 10)
			if i != len(skuidVec) - 1 {
				pinStr += ","
			}
		}
		pinStr = pinStr[0:len(pinStr) - 1]
		pinStr += ")"
		selectStr = "select " + SelectSkuField + " from t_sku where Fskuid in " + pinStr
	}
	fmt.Println(selectStr)
	rows, err := db.Query(selectStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	var sqlNum = 0
	for rows.Next() {
		var skuInfo SourceSku
		_, _ = rows.Columns()
		err = rows.Scan(&skuInfo.Fskuid,&skuInfo.Fcooperatorsubaccountid,&skuInfo.Fskulocalcode,&skuInfo.Fcooperatorid,&skuInfo.Fcategoryid,&skuInfo.Fskutype,&skuInfo.Fskuoperationmodel,
			&skuInfo.Fbarcode,&skuInfo.Fskudimensionalbarcode,&skuInfo.Fskutitle,&skuInfo.Fskusubtitle,&skuInfo.Fskupromotdesc,&skuInfo.Fskusaleattr,&skuInfo.Fskusaleattrdesc,
			&skuInfo.Fskukeyattr, &skuInfo.Fskukeyattrdesc,&skuInfo.Fskureferprice,&skuInfo.Fskupurchprice,&skuInfo.Fskuproperty,&skuInfo.Fskuproperty1,&skuInfo.Fskustate,&skuInfo.Fskuweight,
			&skuInfo.Fskunetweight,&skuInfo.Fskuvolume,&skuInfo.Fskusizex,&skuInfo.Fskusizey,&skuInfo.Fskusizez,&skuInfo.Fskucategoryattr,&skuInfo.Fskukeyword,&skuInfo.Fskuvatrate,
			&skuInfo.Fskusnapversion,&skuInfo.Fskubuylimit,&skuInfo.Fskubrand,&skuInfo.Fskuexchangepoint, &skuInfo.Fmainskuid,&skuInfo.Fskulastuptime,&skuInfo.Fskulastdowntime,
			&skuInfo.Fskuaddtime,&skuInfo.Fskulastupdatetime,&skuInfo.Flastupdatetime,&skuInfo.Fskupurchtaxrate,&skuInfo.Fskushortname,&skuInfo.Fskudurability,&skuInfo.Fskutag,
			&skuInfo.Fskusort,&skuInfo.Fskuexpertscomments,&skuInfo.Fskuunit,&skuInfo.Fpopskucode,&skuInfo.Fskueffectivetime,&skuInfo.Fskuexpiretime, &skuInfo.Fskutwodimcodelastprinttime,
			&skuInfo.Fskutwodimcodelastprintperson,&skuInfo.Fskustockexpiretime, &skuInfo.Fskuspecmodel,&skuInfo.Fskuproducingarea,&skuInfo.Fskupackrate,&skuInfo.Fskupacklist,
			&skuInfo.Fskuceilprice,&skuInfo.Fskufloorprice,&skuInfo.Fskuunitcost,&skuInfo.Fskucontacternum,&skuInfo.Fskutravellernum,&skuInfo.Fskusalearea, &skuInfo.Fplatformid,
			&skuInfo.Fcontractid,&skuInfo.Fflagshipstoreid,&skuInfo.Fspuid,&skuInfo.Fcustomserviceid,&skuInfo.Fcertifyurl,&skuInfo.Fcertifyurlid,&skuInfo.Fadpicurl,&skuInfo.Fadlink,
			&skuInfo.Fadbegintime,&skuInfo.Fadendtime,&skuInfo.Fskuservicenum,&skuInfo.Fskuappage,&skuInfo.Fsexid,&skuInfo.Fisforce,&skuInfo.Fcurrency,&skuInfo.Fsource,&skuInfo.Ftoptempid,
			&skuInfo.Fbottomtempid,&skuInfo.Ftempid,&skuInfo.Fmainurl,&skuInfo.Fsocialsofttxt,&skuInfo.Fskunameindex)
		if err != nil {
			fmt.Println("Get sku info failed! [%s]", err)
		}
		sqlNum++
		mapSku[skuInfo.Fskuid] = skuInfo
	}
	fmt.Println("sku sql size=", sqlNum, "realNum=", len(mapSku))
}

func selectSpuField(db *sql.DB, mapSpu map[int64]SourceSpu, spuidVec [500000] int64) {
	//var pinStr = ""
	var spuStr string
	spuStr = "select " + SelectSpuField + " from t_spu"
	/*if spuidVec[0] != 0 {
		pinStr += "("
		for i := 0; i < len(spuidVec); i++ {
			if spuidVec[i] == 0 {
				continue
			}
			pinStr += strconv.FormatInt(spuidVec[i], 10)
			if i != len(spuidVec) - 1 {
				pinStr += ","
			}
		}
		pinStr = pinStr[0:len(pinStr) - 1]
		pinStr += ")"
		spuStr = "select " + SelectSpuField + " from t_spu where Fspuid in " + pinStr
	}*/
	rows, err := db.Query(spuStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	for rows.Next() {
		var spuInfo SourceSpu
		_, _ = rows.Columns()
		err = rows.Scan(&spuInfo.Fspuid, &spuInfo.Fcategoryid, &spuInfo.Foperationmodel, &spuInfo.Fgoodsno, &spuInfo.Fsputype, &spuInfo.Fsputitle, &spuInfo.Fspupromotdesc,
			&spuInfo.Fspupromotdescefftime, &spuInfo.Fspupromotdescexptime, &spuInfo.Fspubrand, &spuInfo.Fspuspecmodel, &spuInfo.Fspureferprice, &spuInfo.Fspuprice, &spuInfo.Fspuproperty,
			&spuInfo.Fspuproperty1, &spuInfo.Fspucategoryattr, &spuInfo.Fspuweight, &spuInfo.Fspusizex, &spuInfo.Fspusizey, &spuInfo.Fspusizez, &spuInfo.Fspuunit, &spuInfo.Fspupackrate,
			&spuInfo.Fspupacklist, &spuInfo.Fspustate, &spuInfo.Foperatetime, &spuInfo.Freason, &spuInfo.Faddtime, &spuInfo.Flastupdatetime, &spuInfo.Fcooperatorid, &spuInfo.Fspuappage,
			&spuInfo.Fsexid, &spuInfo.Ftempid, &spuInfo.Ftempid , &spuInfo.Ftoptempid , &spuInfo.Fbottomtempid , &spuInfo.Fisforce , &spuInfo.Fsocialsofttxt)
		if err != nil {
			fmt.Println("Get spu info failed! [%s]", err)
		}
		mapSpu[spuInfo.Fspuid] = spuInfo
	}
}

func selectAreaField(db *sql.DB, mapArea map[string]SoureArea) {
	var areaStr string
	areaStr = "select " + SelectAreaField + " from t_sku_area"
	rows, err := db.Query(areaStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	var key string
	var moreKey string
	var sqlNum = 0
	for rows.Next() {
		var areaInfo SoureArea
		_, _ = rows.Columns()
		err = rows.Scan(&areaInfo.Fskuid,&areaInfo.Fcooperatorsubaccountid,&areaInfo.Fcooperatorskuareacode,&areaInfo.Fcooperatorbarcode,&areaInfo.Fpromotiontype,
			&areaInfo.Fskuareapromotdesc,&areaInfo.Fskuareaprice,&areaInfo.Fskuareapreprice,&areaInfo.Fskuareacostprice,&areaInfo.Fskuareabusinesscost,
			&areaInfo.Fskuareaestimatedispatch,&areaInfo.Fskuareaproperty,&areaInfo.Fskuareastate,&areaInfo.Fskuareaminbuycount,
			&areaInfo.Fskuareamaxbuycount,&areaInfo.Fskuareaaddtime, &areaInfo.Fskuarealastupdatetime,&areaInfo.Fskuareabuymultiple,&areaInfo.Fskuareastorelogistictype,
			&areaInfo.Fskuareafirstpublishtime,&areaInfo.Fskuareapromotdescefftime,&areaInfo.Fskuareapromotdescexptime,&areaInfo.Fskuareabuyer,&areaInfo.Fskuareasharecommission,
			&areaInfo.Fentityid,&areaInfo.Fstoreid,&areaInfo.Fchannelid,&areaInfo.Fdetail,&areaInfo.Fworkstatecode,&areaInfo.Fcirculationmodecode,&areaInfo.Fskuareapostprice,
			&areaInfo.Fincatalog,&areaInfo.Fareasort,&areaInfo.Fsaleslabelid,&areaInfo.Fsaleslabelname,&areaInfo.Fsaleslabelurl,&areaInfo.Fsalesstarttime,&areaInfo.Fsalesendtime,
			&areaInfo.Fstorestate,&areaInfo.Fsharecommissionbegintime,&areaInfo.Fsharecommissionendtime,&areaInfo.Fcooperatorid,&areaInfo.Fisinstall,&areaInfo.Finstallcost,
			&areaInfo.Fcontractid)
		sqlNum++
		if err != nil {
			fmt.Println("Get area info failed! [%s]", err)
		}
		key = strconv.FormatInt(areaInfo.Fskuid, 10) + "|" + strconv.FormatInt(int64(areaInfo.Fentityid), 10)
		_, ok := mapArea[key]
		if ok {
			moreKey += key +  ","
		} else {
			mapArea[key] = areaInfo
		}
	}
	fmt.Println("area sql size=", sqlNum, "realNum=", len(mapArea))
}

func newAreaField(db *sql.DB, mapArea map[string]NewArea, index string , tablenum int) {
	var areaStr string
	locate, err := strconv.Atoi(index[2:])
	areaStr = "select " + NewAreaField + " from t_sku_area_" + strconv.FormatInt(int64(locate%tablenum), 10)  + " where Fspuid = '" + index + "';"
	rows, err := db.Query(areaStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	var key string
	for rows.Next() {
		var areaInfo NewArea
		_, _ = rows.Columns()
		err = rows.Scan(&areaInfo.Fplatformid, &areaInfo.Fspuid,&areaInfo.Fskuid,&areaInfo.Fentityid,&areaInfo.Fchannelid,&areaInfo.Fstoreid,&areaInfo.Fincatalog,
			&areaInfo.Fworkstatecode,&areaInfo.Fcirculationmodecode,&areaInfo.Fskuareapostprice, &areaInfo.Fskuareaprice,&areaInfo.Fskuareapreprice,&areaInfo.Fsaleslabelid,
			&areaInfo.Fsubchannelinfo,&areaInfo.Fskuareaproperty,&areaInfo.Fskuareabuyer, &areaInfo.Fskuareaaddtime,&areaInfo.Fskuarealastupdatetime,&areaInfo.Fskuareafirstpublishtime)
		if err != nil {
			fmt.Println("Get area info failed! [%s]", err)
		}
		key = strconv.FormatInt(areaInfo.Fskuid, 10) + "|" + strconv.FormatInt(int64(areaInfo.Fentityid), 10)
		mapArea[key] = areaInfo
	}
}

func newSpuField(db *sql.DB, mapNewSpu map[string]NewSpu, index string, tablenum int) {
	var spuStr string
	if len(index) == 1 {
		locate, _ := strconv.Atoi(index)
		spuStr = "select " + NewSpuField + " from t_spu_" + strconv.FormatInt(int64(locate%tablenum), 10)
	} else {
		locate, _ := strconv.Atoi(index[2:])
		spuStr = "select " + NewSpuField + " from t_spu_" + strconv.FormatInt(int64(locate%tablenum), 10) + " where Fspuid = '" + index + "';"
	}
	rows, err := db.Query(spuStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	for rows.Next() {
		var spuInfo NewSpu
		_, _ = rows.Columns()
		err = rows.Scan(&spuInfo.Fplatformid,&spuInfo.Fspuid,&spuInfo.Fspuextcode,&spuInfo.Fmainspuid,&spuInfo.Fcooperatorid,&spuInfo.Fflagshipstoreid,&spuInfo.Fsputitle,
			&spuInfo.Fspuonlinetitle,&spuInfo.Fspushortname,&spuInfo.Fspusubtitle,&spuInfo.Fspustate, &spuInfo.Fspuoperationmodel,&spuInfo.Fsputype,&spuInfo.Fspumainurl,&spuInfo.Fbrandid,
			&spuInfo.Ferpbrandid,&spuInfo.Fcategoryid,&spuInfo.Ferppubnavid,&spuInfo.Fspusaleattr,&spuInfo.Fspucategoryattr,&spuInfo.Ftempid,&spuInfo.Ftoptempid,&spuInfo.Fbottomtempid,
			&spuInfo.Fspuproperty,&spuInfo.Fspuvatrate,&spuInfo.Fspupurchtaxrate,&spuInfo.Fspuproducingarea,&spuInfo.Fspuspecmodel,&spuInfo.Fspusort,&spuInfo.Fsaleslabelid,&spuInfo.Fmcid,
			&spuInfo.Fremark,&spuInfo.Fcreateuser,&spuInfo.Fspuaddtime,&spuInfo.Fspulastupdatetime,&spuInfo.Fspulastupstoretime,&spuInfo.Fspulastdownstoretime)
		if err != nil {
			fmt.Println("Get area info failed! [%s]", err)
		}
		mapNewSpu[spuInfo.Fspuid] = spuInfo
	}
}

func newSkuField(db *sql.DB, mapSku map[int64]NewSku, index string, tablenum int) {
	var areaStr string
	locate, err := strconv.Atoi(index[2:])
	areaStr = "select " + NewSkuField + " from t_sku_" + strconv.FormatInt(int64(int), 10)  + " where Fspuid = '" + index + "';"
	rows, err := db.Query(areaStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	for rows.Next() {
		var skuInfo NewSku
		_, _ = rows.Columns()
		err = rows.Scan(&skuInfo.Fplatformid,&skuInfo.Fspuid ,&skuInfo.Fskuid ,&skuInfo.Fskuextcode,&skuInfo.Fskulocalcode,&skuInfo.Fgoodsno ,&skuInfo.Fskustate,
			&skuInfo.Fcategoryattr,&skuInfo.Fskusaleattr ,&skuInfo.Fskukeyattr,&skuInfo.Fskupacklist ,&skuInfo.Fskumainurl,&skuInfo.Fskuminbuycount,&skuInfo.Fskubuymultiple,
			&skuInfo.Fskumaxbuycount,&skuInfo.Fskuunitcost ,&skuInfo.Fskuerpreferprice,&skuInfo.Fskuerppurchprice,&skuInfo.Fskuerpceilingprice,&skuInfo.Fskuerpfloorprice,
			&skuInfo.Fgrossmargin ,&skuInfo.Fskuproperty ,&skuInfo.Fskuunit ,&skuInfo.Fskulevel, &skuInfo.Fskustockexpiretime,&skuInfo.Fskuaddtime, &skuInfo.Fskulastupdatetime,
			&skuInfo.Fskulastupstatetime, &skuInfo.Fskulastdowntime, &skuInfo.Fskufirstpublishtime)
		if err != nil {
			fmt.Println("Get new sku info failed! [%s]", err)
		}
		mapSku[skuInfo.Fskuid] = skuInfo
	}
}

func selectSkuErpField(db *sql.DB, mapErp map[int64]SkuErp, skuid int64) {
	var skuerp string
	skuerp = "select " + SkuErpField + " from t_sku_erp where Fskuid =" + strconv.FormatInt(skuid, 10)
	rows, err := db.Query(skuerp)
	if err != nil {
		fmt.Println("select fail [%s]", err)
		return
	}
	var sqlNum = 0
	for rows.Next() {
		var skuerpInfo SkuErp
		_, _ = rows.Columns()
		err = rows.Scan(&skuerpInfo.Fgoodsno, &skuerpInfo.Fgrossmargin, &skuerpInfo.Fskulevel, &skuerpInfo.Fskureferprice, &skuerpInfo.Fskupurchpricev2,
			&skuerpInfo.Fsputitle, &skuerpInfo.Fspuid, &skuerpInfo.Fbrandid, &skuerpInfo.Fcategoryid)
		sqlNum++
		if err != nil {
			fmt.Println("Get erp info failed! [%s]", err)
		}
		mapErp[skuid] = skuerpInfo
	}
}

func checkArea(oldArea SoureArea, newArea NewArea)  {
	var flag bool
	if oldArea.Fchannelid != newArea.Fchannelid {
		flag = true
		fmt.Print(" ,Fchannelid=", oldArea.Fchannelid, newArea.Fchannelid)
	}
	if oldArea.Fstoreid != newArea.Fstoreid {
		flag = true
		fmt.Print(" ,Fstoreid=", oldArea.Fstoreid, newArea.Fstoreid)
	}
	/*if oldArea.Fentityid == 80001 {
		if oldArea.Fskuareastate!= newArea.Fincatalog {
			flag = true
			fmt.Print(" ,Fskuareastate=", oldArea.Fskuareastate, newArea.Fincatalog)
		}
	} else if oldArea.Fchannelid == 2 {
		if oldArea.Fincatalog == 1 && 1 == newArea.Fincatalog {
		} else if 2 == newArea.Fincatalog {
		} else {
			flag = true
			fmt.Println(" ,Fincatalog mmmmmmmmmmmmmmmm=", oldArea.Fincatalog, newArea.Fincatalog)
		}
	}*/
	if oldArea.Fworkstatecode != newArea.Fworkstatecode {
		flag = true
		fmt.Print(" ,Fworkstatecode=", oldArea.Fworkstatecode, newArea.Fworkstatecode)
	}
	if oldArea.Fcirculationmodecode != newArea.Fcirculationmodecode {
		flag = true
		fmt.Print(" ,Fcirculationmodecode=", oldArea.Fcirculationmodecode, newArea.Fcirculationmodecode)
	}
	if oldArea.Fskuareapostprice != newArea.Fskuareapostprice {
		flag = true
		fmt.Print(" ,Fskuareapostprice=", oldArea.Fskuareapostprice, newArea.Fskuareapostprice)
	}
	if oldArea.Fskuareaprice != newArea.Fskuareaprice {
		flag = true
		fmt.Print(" ,Fskuareaprice=", oldArea.Fskuareaprice, newArea.Fskuareaprice)
	}
	if oldArea.Fskuareapreprice != newArea.Fskuareapreprice {
		flag = true
		fmt.Print(" ,Fskuareapreprice=", oldArea.Fskuareapreprice, newArea.Fskuareapreprice)
	}
	if strings.Compare(oldArea.Fsaleslabelid.String, newArea.Fsaleslabelid.String) != 0 {
		flag = true
		fmt.Print(" ,Fsaleslabelid=", oldArea.Fsaleslabelid, newArea.Fsaleslabelid)
	}
	if strings.Compare(oldArea.Fskuareabuyer, newArea.Fskuareabuyer) != 0 {
		flag = true
		fmt.Print(" ,Fskuareabuyer=", oldArea.Fskuareabuyer, newArea.Fskuareabuyer)
	}
	if flag {
		fmt.Print("      ,Fskuid ", oldArea.Fskuid, newArea.Fentityid,  time.Unix(oldArea.Fskuarealastupdatetime, 0).Format("2006-01-02 15:04:05"))
		fmt.Println("")
	}
}

func getSpupicture(db *sql.DB, spuid int64) string {
	var mainurl string
	picStr := "select Fpicurl  from t_spu_picture where Fskuid = '" + strconv.FormatInt(spuid, 10) + "' and Fskupicindex = 0;"
	rows, err := db.Query(picStr)
	if err != nil {
		fmt.Println("select fail [%s]", err)
	}
	for rows.Next() {
		_, _ = rows.Columns()
		err = rows.Scan(&mainurl)
		if err != nil {
			fmt.Println("Get erp info failed! [%s]", err)
		}
	}
	return mainurl
}

func checkSku(db *sql.DB,sku SourceSku, newSku NewSku) {
	var flag bool
	/*if sku.Fcooperatorid != newSku.Fplatformid { //不要了
		flag = true
		fmt.Print(" ,Fplatformid=", sku.Fcooperatorid, newSku.Fplatformid)
	}*/
	if sku.Fpopskucode != newSku.Fskuextcode {
		flag = true
		fmt.Print(" ,Fpopskucode=", sku.Fpopskucode, newSku.Fskuextcode)
	}
	if sku.Fskulocalcode != newSku.Fskulocalcode {
		flag = true
		fmt.Print(" ,Fskulocalcode=", sku.Fskulocalcode, newSku.Fskulocalcode)
	}
	if sku.Fskustate != newSku.Fskustate {
		//flag = true
		//fmt.Print(" ,Fskustate=", sku.Fskustate, newSku.Fskustate)
	}
	if sku.Fskucategoryattr != newSku.Fcategoryattr {
		flag = true
		fmt.Print(" ,Fcategoryattr=", sku.Fskucategoryattr, newSku.Fcategoryattr)
	}
	if sku.Fskusaleattr != newSku.Fskusaleattr {
		flag = true
		fmt.Print(" ,Fskusaleattr=", sku.Fskusaleattr, newSku.Fskusaleattr)
	}
	if sku.Fskukeyattr != newSku.Fskukeyattr {
		flag = true
		fmt.Print(" ,Fskukeyattr=", sku.Fskukeyattr, newSku.Fskukeyattr)
	}
	if sku.Fskupacklist != newSku.Fskupacklist {
		flag = true
		fmt.Print(" ,Fskupacklist=", sku.Fskupacklist, newSku.Fskupacklist)
	}
	if sku.Fspuid == 0 {
		if sku.Fmainurl != newSku.Fskumainurl {
			flag = true
			fmt.Print(" ,Fskumainurl=", sku.Fmainurl, newSku.Fskumainurl)
		}
	}
	var areakey = strconv.FormatInt(sku.Fskuid, 10) + "|" + "8000"
    _, ok := mapArea[areakey]
    if ok {
    	if newSku.Fskuminbuycount != mapArea[areakey].Fskuareaminbuycount {
			flag = true
			fmt.Print(" ,Fskuminbuycount=", mapArea[areakey].Fskuareaminbuycount, newSku.Fskuminbuycount)
		}
		if newSku.Fskubuymultiple != mapArea[areakey].Fskuareabuymultiple {
			flag = true
			fmt.Print(" ,Fskubuymultiple=", mapArea[areakey].Fskuareabuymultiple, newSku.Fskubuymultiple)
		}
		if newSku.Fskumaxbuycount != mapArea[areakey].Fskuareamaxbuycount {
			flag = true
			fmt.Print(" ,Fskumaxbuycount=", mapArea[areakey].Fskuareamaxbuycount, newSku.Fskumaxbuycount)
		}
	}
	_, ok = mapSkuErp[sku.Fskuid]
	if ok {
		if newSku.Fgrossmargin != mapSkuErp[sku.Fskuid].Fgrossmargin {
			flag = true
			fmt.Print(" ,Fgrossmargin=",  mapSkuErp[sku.Fskuid], newSku.Fgrossmargin)
		}
		if newSku.Fgoodsno != mapSkuErp[sku.Fskuid].Fgoodsno {
			flag = true
			fmt.Print(" ,Fgoodsno=", mapSkuErp[sku.Fskuid].Fgoodsno, newSku.Fgoodsno)
		}
		if newSku.Fskulevel != mapSkuErp[sku.Fskuid].Fskulevel {
			flag = true
			fmt.Print(" ,Fskulevel=", mapSkuErp[sku.Fskuid].Fskulevel, newSku.Fskulevel)
		}
		if mapSkuErp[sku.Fskuid].Fskupurchpricev2 != newSku.Fskuerppurchprice {
			flag = true
			fmt.Print(" ,Fskupurchpricev2=", mapSkuErp[sku.Fskuid].Fskupurchpricev2, newSku.Fskuerppurchprice)
		}
	}
	if (sku.Fskureferprice * 100) != newSku.Fskuerpreferprice {
		flag = true
		fmt.Print(" ,Fskureferprice=", mapSkuErp[sku.Fskuid].Fskureferprice * 100, newSku.Fskuerpreferprice)
	}
	if sku.Fskuunitcost != newSku.Fskuunitcost {
		flag = true
		fmt.Print(" ,Fskuunitcost=", sku.Fskuunitcost, newSku.Fskuunitcost)
	}
	if sku.Fskuceilprice != newSku.Fskuerpceilingprice {
		flag = true
		fmt.Print(" ,Fskuerpceilingprice=", sku.Fskuceilprice, newSku.Fskuerpceilingprice)
	}
	if sku.Fskufloorprice != newSku.Fskuerpfloorprice {
		flag = true
		fmt.Print(" ,Fskuerpfloorprice=", sku.Fskufloorprice, newSku.Fskuerpfloorprice)
	}
	if sku.Fskuunit != newSku.Fskuunit {
		flag = true
		fmt.Print(" ,Fskuunit=", sku.Fskuunit, newSku.Fskuunit)
	}
	if sku.Fskustockexpiretime != newSku.Fskustockexpiretime {
		flag = true
		fmt.Print(" ,Fskustockexpiretime=", sku.Fskustockexpiretime, newSku.Fskustockexpiretime)
	}
	if sku.Fskulastuptime != newSku.Fskulastupstatetime {
		flag = true
		fmt.Print(" ,Fskulastupstatetime=", sku.Fskulastuptime, newSku.Fskulastupstatetime)
	}
	if sku.Fskulastdowntime != newSku.Fskulastdowntime {
		flag = true
		fmt.Print(" ,Fskulastdowntime=", sku.Fskulastuptime, newSku.Fskulastdowntime)
	}
	if flag {
		fmt.Print("      ,Fskuid ", sku.Fskuid, "   ", time.Unix(sku.Fskulastuptime,0).Format("2006-01-02 15:04:05"))
		fmt.Println("")
	}
}

func checkSpu(db *sql.DB, spu SourceSpu, newSpu NewSpu) { //spu
	var flag bool
	if spu.Fsputitle != newSpu.Fspuonlinetitle {
		flag = true
		fmt.Print(" ,Fspuonlinetitle=", spu.Fsputitle, newSpu.Fspuonlinetitle)
	}
	/*if spu.Fspustate != newSpu.Fspustate {
		flag = true
		fmt.Print(" ,XXXXXXXXXXXXXXXXXXXXXXXXXXXX Fspustate=", spu.Fspustate, newSpu.Fspustate)
	}*/
	if spu.Fsputitle != newSpu.Fspuonlinetitle {
		flag = true
		fmt.Print(" ,Fspuonlinetitle=", spu.Fsputitle, newSpu.Fspuonlinetitle)
	}
	if spu.Fspucategoryattr != newSpu.Fspucategoryattr {
		flag = true
		fmt.Print(" ,Fspucategoryattr=", spu.Fspucategoryattr, newSpu.Fspucategoryattr)
	}
	if spu.Fspuspecmodel != newSpu.Fspuspecmodel {
		flag = true
		fmt.Print(" ,Fspuspecmodel=", spu.Fspuspecmodel, newSpu.Fspuspecmodel)
	}
	if spu.Fsputype != newSpu.Fsputype {
		flag = true
		fmt.Print(" ,Fsputype=", spu.Fsputype, newSpu.Fsputype)
	}

	for _, sku := range mapDomain {
		if sku.Fspuid == spu.Fspuid {
			if sku.Fskushortname != newSpu.Fspushortname {
				flag = true
				fmt.Print(" ,Fsputitle=", sku.Fskushortname, newSpu.Fspushortname)
			}
			if sku.Fskusubtitle != newSpu.Fspusubtitle {
				flag = true
				fmt.Print(" ,Fsputitle=", sku.Fskusubtitle, newSpu.Fspusubtitle)
			}
			if sku.Fskuoperationmodel != newSpu.Fspuoperationmodel {
				flag = true
				fmt.Print(" ,Fsputitle=", sku.Fskuoperationmodel, newSpu.Fspuoperationmodel)
			}
			if sku.Fskubrand != newSpu.Fbrandid {
				flag = true
				fmt.Print(" ,Fskubrand=", sku.Fskubrand, newSpu.Fbrandid)
			}

			if sku.Fcategoryid != newSpu.Fcategoryid {
				flag = true
				fmt.Print(" ,Fcategoryid=", sku.Fcategoryid, newSpu.Fcategoryid)
			}
			if sku.Ftempid != newSpu.Ftempid {
				flag = true
				fmt.Print(" ,Ftempid=", sku.Ftempid, newSpu.Ftempid)
			}
			if sku.Ftoptempid != newSpu.Ftoptempid {
				flag = true
				fmt.Print(" ,Ftoptempid=", sku.Ftoptempid, newSpu.Ftoptempid)
			}
			if sku.Fbottomtempid != newSpu.Fbottomtempid {
				flag = true
				fmt.Print(" ,Fbottomtempid=", sku.Fbottomtempid, newSpu.Fbottomtempid)
			}
			if sku.Fskuvatrate != newSpu.Fspuvatrate {
				flag = true
				fmt.Print(" ,Fskuvatrate=", sku.Fskuvatrate, newSpu.Fspuvatrate)
			}
			if sku.Fskuproducingarea != newSpu.Fspuproducingarea {
				flag = true
				fmt.Print(" ,Fskuproducingarea=", sku.Fskuproducingarea, newSpu.Fspuproducingarea)
			}
			if sku.Fskupurchtaxrate != newSpu.Fspupurchtaxrate {
				flag = true
				fmt.Print(" ,Fskupurchtaxrate=", sku.Fskupurchtaxrate, newSpu.Fspupurchtaxrate)
			}
			erp, ok := mapSkuErp[sku.Fskuid]
			if ok {
				if erp.Fcategoryid != newSpu.Ferppubnavid {
					flag = true
					fmt.Print(" ,Fbrandid=", erp.Fcategoryid, newSpu.Ferppubnavid)
				}
			}
			break
		}
	}
	mainurl := getSpupicture(db, spu.Fspuid)
	if mainurl != newSpu.Fspumainurl {
		flag = true
		fmt.Print(" ,Fspumainurl=", mainurl, newSpu.Fspumainurl)
	}
	if flag {
		fmt.Print("      ,Fspuid ", spu.Fspuid, "   ", time.Unix(spu.Flastupdatetime,0).Format("2006-01-02 15:04:05"))
		fmt.Println("")
	}
}

func checkSpuandSku(sku SourceSku, newSpu NewSpu) {	//sku
	var flag bool
	if sku.Fskutitle != newSpu.Fspuonlinetitle {
		flag = true
		fmt.Print(" ,Fskutitle=", sku.Fskutitle, newSpu.Fsputitle)
	}
	if sku.Fskutype != newSpu.Fsputype {
		flag = true
		fmt.Print(" ,Fsputype=", sku.Fskutype, newSpu.Fsputype)
	}
	if sku.Fmainurl != newSpu.Fspumainurl {
		flag = true
		fmt.Print(" ,Fsputitle=", sku.Fmainurl, newSpu.Fspumainurl)
	}
	if sku.Fskushortname != newSpu.Fspushortname {
		flag = true
		fmt.Print(" ,Fsputitle=", sku.Fskushortname, newSpu.Fspushortname)
	}
	if sku.Fskusubtitle != newSpu.Fspusubtitle {
		flag = true
		fmt.Print(" ,Fsputitle=", sku.Fskusubtitle, newSpu.Fspusubtitle)
	}
	if sku.Fskuoperationmodel != newSpu.Fspuoperationmodel {
		flag = true
		fmt.Print(" ,Fsputitle=", sku.Fskuoperationmodel, newSpu.Fspuoperationmodel)
	}
	if sku.Fskubrand != newSpu.Fbrandid {
		flag = true
		fmt.Print(" ,Fskubrand=", sku.Fskubrand, newSpu.Fbrandid)
	}
	if sku.Fcategoryid != newSpu.Fcategoryid {
		flag = true
		fmt.Print(" ,Fcategoryid=", sku.Fcategoryid, newSpu.Fcategoryid)
	}
	if sku.Ftempid != newSpu.Ftempid {
		flag = true
		fmt.Print(" ,Ftempid=", sku.Ftempid, newSpu.Ftempid)
	}
	if sku.Ftoptempid != newSpu.Ftoptempid {
		flag = true
		fmt.Print(" ,Ftoptempid=", sku.Ftoptempid, newSpu.Ftoptempid)
	}
	if sku.Fbottomtempid != newSpu.Fbottomtempid {
		flag = true
		fmt.Print(" ,Fbottomtempid=", sku.Fbottomtempid, newSpu.Fbottomtempid)
	}
	if sku.Fskuvatrate != newSpu.Fspuvatrate {
		flag = true
		fmt.Print(" ,Fskuvatrate=", sku.Fskuvatrate, newSpu.Fspuvatrate)
	}
	if sku.Fskuproducingarea != newSpu.Fspuproducingarea {
		flag = true
		fmt.Print(" ,Fskuproducingarea=", sku.Fskuproducingarea, newSpu.Fspuproducingarea)
	}
	if sku.Fskupurchtaxrate != newSpu.Fspupurchtaxrate {
		flag = true
		fmt.Print(" ,Fskupurchtaxrate=", sku.Fskupurchtaxrate, newSpu.Fspupurchtaxrate)
	}
	erp, ok := mapSkuErp[sku.Fskuid]
	if ok {
		if erp.Fcategoryid != newSpu.Ferppubnavid {
			flag = true
			fmt.Print(" ,Fbrandid=", erp.Fcategoryid, newSpu.Ferppubnavid)
		}
	}
	//状态 skustate -> spu state
	/*if sku.Fskustate != newSpu.Fspustate {
		if sku.Fskustate == 1 && newSpu.Fspustate != 5 {
			flag = true
			fmt.Print(" ,Fskustate22=", sku.Fskustate, newSpu.Fspustate)
		} else if sku.Fskustate == 2 && newSpu.Fspustate != 6 {
			flag = true
			fmt.Print(" ,Fskustate33=", sku.Fskustate, newSpu.Fspustate)
		}
	}*/
	if flag {
		fmt.Print("    skuid  ,Fspuid ", sku.Fskuid, "xxyy ", newSpu.Fspuid, " ")
		fmt.Println("")
	}
}

func checkdata(db *sql.DB, db2 *sql.DB, tablenum int) {
	mapDomain = make(map[int64]SourceSku)
	mapSpu = make(map[int64]SourceSpu)
	mapArea = make(map[string]SoureArea)
	mapNewArea = make(map[string]NewArea)
	mapNewSku = make(map[int64]NewSku)
	mapSkuErp = make(map[int64]SkuErp)
	mapNewSpu = make(map[string]NewSpu)
	var spuVec [3000000]string
	var spuidVec [500000]int64
	var skuVec [2000]int64
		/*selectAreaField(db, mapArea)
		i := 0
		for _, area := range mapArea {
			skuVec[i] = area.Fskuid
			i++
			selectSkuErpField(db, mapSkuErp, area.Fskuid)
		}*/
		selectSkuField(db, mapDomain, skuVec)
		i := 0
		j := 0
		var nullstr = "0000000000"
		for _, sku := range mapDomain {
			if sku.Fspuid != 0 {
				str := strconv.FormatInt(sku.Fspuid, 10)
				spuVec[j] = "SP" + nullstr[0:len(nullstr)-len(str)] + str
				spuidVec[i] = sku.Fspuid
				i++
			} else {
				str := strconv.FormatInt(sku.Fskuid, 10)
				spuVec[j] = "SK" + nullstr[0:len(nullstr)-len(str)] + str
			}
			j++
		}
		selectSpuField(db, mapSpu, spuidVec)
		for i := 0; i < len(spuVec); i++ {
			if len(spuVec[i]) == 0 {
				break
			}
			//newAreaField(db2, mapNewArea, spuVec[i], tablenum * 2)
			newSkuField(db2, mapNewSku, spuVec[i], tablenum)
			newSpuField(db2, mapNewSpu, spuVec[i], tablenum)
		}
		fmt.Println("checkArea=========================================================================================================")
		/*for areaKey, _ := range mapArea {
			newArea, ok := mapNewArea[areaKey]
			if ok {
				checkArea(mapArea[areaKey], newArea)
			}
		}*/
		fmt.Println("checkSku=========================================================================================================")
		for skuKey, _ := range mapDomain {
			newSku, ok := mapNewSku[skuKey]
			if ok {
				checkSku(db, mapDomain[skuKey], newSku)
			}
		}
		fmt.Println("checkSpu=========================================================================================================")
		for spuKey, _ := range mapNewSpu {
			spuOrsku, _ := strconv.Atoi(spuKey[2:])
			if strings.Contains(spuKey, "SP") {
				checkSpu(db, mapSpu[int64(spuOrsku)], mapNewSpu[spuKey])
			} else if strings.Contains(spuKey, "SK") {
				skuKey, _ := mapDomain[int64(spuOrsku)]
				checkSpuandSku(skuKey, mapNewSpu[spuKey])
			} else {
				os.Exit(-1)
			}
		}

	fmt.Println("sku     size=", len(mapDomain))
	fmt.Println("spu     size=", len(mapSpu))
	fmt.Println("area    size=", len(mapArea))
	fmt.Println("erp     size=", len(mapSkuErp))
	fmt.Println("newarea size=", len(mapNewArea))
	fmt.Println("newsku  size=", len(mapNewSku))
	fmt.Println("newspu  size=", len(mapNewSpu))
}

func checkskuid(db *sql.DB, db2 *sql.DB, tablenum int) {
	var mapSku1 map[int64]int64
	var mapSpu2 map[int64]int64
	mapSku1 = make(map[int64] int64)
	mapSpu2 = make(map[int64] int64)

	var selectStr = "select Fskuid from t_sku"
	rows, err := db.Query(selectStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	for rows.Next() {
		var skuid int64 
		_, _ = rows.Columns()
		err = rows.Scan(&skuid)
		if err != nil {
			fmt.Println("Get sku info failed! [%s]", err)
		}
		mapSku1[skuid] = skuid
	}

	for i := 0; i < tablenum; i++ {
		var areaStr string
		areaStr = "select Fskuid from t_sku_" + strconv.FormatInt(int64(i), 10) + ";"
		fmt.Println(areaStr);
		rows, err := db2.Query(areaStr)
		if err != nil{
			fmt.Println("select fail [%s]",err)
			return
		}
		for rows.Next() {
			var skuidSpu int64
			_, _ = rows.Columns()
			err = rows.Scan(&skuidSpu)
			if err != nil {	
				fmt.Println("Get new sku info failed! [%s]", err)
			}
			mapSpu2[skuidSpu] = skuidSpu
		}
	}
	fmt.Println("oldsku  size=", len(mapSku1))
	fmt.Println("newsku  size=", len(mapSpu2))
	fmt.Println("desc  size=", len(mapSku1) - len(mapSpu2))
	for skuidKey, _ := range mapSku1 {
		_, ok := mapSpu2[skuidKey]
		if !ok {
			fmt.Print(skuidKey,",")		
		}
	}
}

func checkdata2(db *sql.DB, db2 *sql.DB, tablenum int) {
	for i := 0; i < tablenum; i++ {
		var mapOldSKu1 map[int64]SourceSku
		var mapNewSku1 map[int64]NewSku
		mapNewSku1 = make(map[int64]NewSku)
		mapOldSKu1 = make(map[int64]SourceSku)
		newSkuField(db2);

		SelectSkuField
		var areaStr string
		areaStr = "select Fskuid from t_sku_" + strconv.FormatInt(int64(i), 10) + ";"
		fmt.Println(areaStr);
		rows, err := db2.Query(areaStr)
		if err != nil{
			fmt.Println("select fail [%s]",err)
			return
		}
		for rows.Next() {
			var skuidSpu int64
			_, _ = rows.Columns()
			err = rows.Scan(&skuidSpu)
			if err != nil {	
				fmt.Println("Get new sku info failed! [%s]", err)
			}
			mapSpu2[skuidSpu] = skuidSpu
		}
	}

	mapDomain = make(map[int64]SourceSku)
	mapSpu = make(map[int64]SourceSpu)
	mapArea = make(map[string]SoureArea)
	mapNewArea = make(map[string]NewArea)
	mapNewSku = make(map[int64]NewSku)
	mapSkuErp = make(map[int64]SkuErp)
	mapNewSpu = make(map[string]NewSpu)
	var spuVec [3000000]string
	var spuidVec [500000]int64
	var skuVec [2000]int64
	var mapSku1 map[int64]int64
	var mapSpu2 map[int64]int64
	mapSku1 = make(map[int64] int64)
	mapSpu2 = make(map[int64] int64)

	var selectStr = "select Fskuid from t_sku"
	rows, err := db.Query(selectStr)
	if err != nil{
		fmt.Println("select fail [%s]",err)
		return
	}
	for rows.Next() {
		var skuid int64 
		_, _ = rows.Columns()
		err = rows.Scan(&skuid)
		if err != nil {
			fmt.Println("Get sku info failed! [%s]", err)
		}
		mapSku1[skuid] = skuid
	}

	for i := 0; i < tablenum; i++ {
		var areaStr string
		areaStr = "select Fskuid from t_sku_" + strconv.FormatInt(int64(i), 10) + ";"
		fmt.Println(areaStr);
		rows, err := db2.Query(areaStr)
		if err != nil{
			fmt.Println("select fail [%s]",err)
			return
		}
		for rows.Next() {
			var skuidSpu int64
			_, _ = rows.Columns()
			err = rows.Scan(&skuidSpu)
			if err != nil {	
				fmt.Println("Get new sku info failed! [%s]", err)
			}
			mapSpu2[skuidSpu] = skuidSpu
		}
	}
	fmt.Println("oldsku  size=", len(mapSku1))
	fmt.Println("newsku  size=", len(mapSpu2))
	fmt.Println("desc  size=", len(mapSku1) - len(mapSpu2))
	for skuidKey, _ := range mapSku1 {
		_, ok := mapSpu2[skuidKey]
		if !ok {
			fmt.Print(skuidKey,",")		
		}
	}

	fmt.Println("sku     size=", len(mapDomain))
	fmt.Println("spu     size=", len(mapSpu))
	fmt.Println("area    size=", len(mapArea))
	fmt.Println("erp     size=", len(mapSkuErp))
	fmt.Println("newarea size=", len(mapNewArea))
	fmt.Println("newsku  size=", len(mapNewSku))
	fmt.Println("newspu  size=", len(mapNewSpu))
}

func main() {
	fmt.Println("Hello, World!\n", time.Now())
	
	db, _ := sql.Open("mysql", "goods:TxjstJdlx@tcp(172.25.0.152:3306)/commodity?charset=utf8")
	//db, _ := sql.Open("mysql", "test_db:test_code@tcp(172.172.178.18:3309)/commodity?charset=utf8")
	if err := db.Ping(); err != nil {
		fmt.Println("open databases fail")
		return
	}
	fmt.Println("connect commodity success")
	db2, _ := sql.Open("mysql", "commodit_canal:1BRN2XrSnHqaQ1u3@tcp(172.25.2.196:3306)/commodity_spu?charset=utf8")
	//db2, _ := sql.Open("mysql", "test_db:test_code@tcp(172.172.178.18:3309)/commodity_spu?charset=utf8")
	if err := db2.Ping(); err != nil {
		fmt.Println("open commodity_spu fail")
		return
	}
	fmt.Println("connect commodity_spu success")
	
    var execIndex int
	flag.IntVar(&execIndex, "t", 1, "")
	flag.Parse()
	fmt.Println("执行参数 ", execIndex);
	if execIndex == 1 {
		checkdata(db, db2, 32)
	} else {
		checkskuid(db, db2, 32)
	}

	fmt.Println(time.Now())
	os.Exit(0)
}


