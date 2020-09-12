package main
/*
   create by bianbian, 2020/6/2
*/

import (
	"database/sql"
	"fmt"
	"io"
	"os"
	"strconv"
	_ "strings"
	"time"
	_ "github.com/go-sql-driver/mysql"
	_ "github.com/Luxurioust/excelize"
	"encoding/csv"
)

var printlnlog int
//包还要排序

func SelectSkuFieldo(db *sql.DB, mapSku map[int64]string, skuVec [] int64)  {
	var pinStr = ""
	//var selectStr = "select " + SelectSkuField + " from t_sku Order By Flastupdatetime Desc limit 100"
	var selectStr = "select * from t_sku"

	if len(skuVec) != 0 {
		pinStr += "("
		for i := 0; i < len(skuVec); i++ {
			if skuVec[i] == 0 {
				continue
			}
			pinStr += strconv.FormatInt(skuVec[i], 10)
			if i != len(skuVec) - 1 {
				pinStr += ","
			}
		}
		pinStr = pinStr[0:len(pinStr) - 1]
		pinStr += ")"
		selectStr = "select * from t_sku where Fskuid in " + pinStr
	}
	//fmt.Println(selectStr)
	rows, err := db.Query(selectStr)
	if err != nil{
		fmt.Println(err)
		return
	}
	var sqlNum = 0
	for rows.Next() {
		var skuInfo string
		_, _ = rows.Columns()
		err = rows.Scan(&skuInfo);
		if err != nil {
			fmt.Println(err)
		}
		sqlNum++
		mapSku[10] = skuInfo
	}
	fmt.Println("sku sql size=", sqlNum, "realNum=", len(mapSku))
}

func ReadExec(filepath string) {
	file, err := os.Open(filepath)
    if err != nil {
        fmt.Println("Error:", err)
        return
	}
	defer file.Close()
	sqlStr := "update t_sku set Fskukeyattr = "
    reader := csv.NewReader(file)
    for {
        record, err := reader.Read()  //recode 是数组 真叼
        if err == io.EOF {
            break
        } else if err != nil {
            fmt.Println("Error:", err)
            return
		}
		salepoint := string(record[1])
		if record[2] != "" {
			salepoint += ";" + string(record[2])
		}
		if record[3] != "" {
			salepoint += ";" + string(record[3])
		}
		allSql := sqlStr + "'" + salepoint + "' where Fskuid = " + string(record[0]) + ";"
		fmt.Println(allSql)
	}
}

func main() {
	fmt.Println("Hello, World!\n", time.Now())

	argsWithProg := os.Args
    if(len(argsWithProg) < 2) {
        fmt.Println("usage : ./readExec xxx.xls", argsWithProg[0])
        return
	}
	confFile := os.Args[1]
    fmt.Println(argsWithProg)
	fmt.Println("执行参数 ", confFile);

	ReadExec(confFile);

	fmt.Println(time.Now())
	os.Exit(0)
}


