//
// Created by nodove on 24. 11. 9.
//

/*
 * compiler 가 삽입하는 디폴트 복사 생성자 코드는 무엇인가?
 * */


//  포인터 주소만 복사하는 얕은 복사
Book(const Book& other) {
    this->title = other.title;
    this->price = other.price;
}