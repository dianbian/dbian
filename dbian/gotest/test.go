/*
   create by bianbian, 2020/6/2
*/

package main

//包还要排序
import (
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
)


func main() {
	fmt.Println("Hello, World!")

	db, _ := sql.Open("mysql", "test_db:test_code@tcp(172.172.178.18:3309)/commodity?charset=utf8")

	if err := db.Ping(); err != nil {
		fmt.Println("open databases fail")
		return
	}
	fmt.Println("connect success")
}
