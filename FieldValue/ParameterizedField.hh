#ifndef ParameterizedField_H
#define ParameterizedField_H 1
//https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=sharonichoya&logNo=220507818075
//전처리기, 헤더파일 중복 방지 조건문
//in not define <header> 만약 헤더가 정의되어 있지 않다면
//define <header> 헤더를 정의하라

#include <stdio.h>
using namespace std;

class ParameterizedField
//https://coding-factory.tistory.com/697
//클래스 만들기
//https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=gnsehfvlr&logNo=120198605191

{
	private: 
		double x;
		double y;
		double z;

	public:
		ParameterizedField();
		//https://stackoverflow.com/questions/47092536/c-error-definition-of-implicitly-declared
		//Error message
		// error: definition of implicitly-declared ‘ParameterizedField::ParameterizedField()’

		~ParameterizedField();
		void SetCurrentPosition( const double Setx, const double Sety, const double Setz );
	//https://haengpeu.tistory.com/154, 접근자와 설정자
	//https://agh2o.tistory.com/8, const 인자 ( 멤버 함수 내부에서 인자를 변형하지 못하게 함 )
	//https://easycoding91.tistory.com/entry/C-%EA%B0%95%EC%A2%8C-const-%EC%9C%84%EC%B9%98%EC%9D%98-%EC%9D%98%EB%AF%B8%EC%99%80-%EC%82%AC%EC%9A%A9-%EB%B0%A9%EB%B2%95
		double* GetParaField() const;
		//const 멤버함수, 이 함수 내에서 멤버 변수들의 변경이 불가함
		//https://zetawiki.com/wiki/C%EC%96%B8%EC%96%B4_%EB%B0%B0%EC%97%B4_%EB%B0%98%ED%99%98%ED%95%98%EA%B8%B0 , 배열 return하기

};

#endif
