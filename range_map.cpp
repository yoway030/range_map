#include <iostream>
#include "range_map.h"

int main()
{
	/***********************************************
	범위저장 컨테이너
	Key가 1~10와 같은 범위형태

	1. Key의 변경형태 (값은 동일하다고 가정할때)
	1~10이 들어있을때
		5로 찾으면 1~10이 나옴.
		0~3 으로 찾아도 1~10이 나옴
		6~100 으로 찾아도 1~10이 나옴

	1이 들어있을때
		2를 넣으면 1~2로 들어감
		0~5를 넣으면 0~5로 들어감

	10~20이 들어있을때
		15를 삭제하면 10~14, 16~20 으로 갈라짐

	10~20, 30~40 이 들어있을때
		15~35를 넣으면 값이 동일할 경우. 10~40이 됨
		값이 다를 경우 10~14,15~35,36~40 으로 갈림


	특정키로 넣을대 인접값이 존재하면 합치는 동작은 정수만 가능.
	특정키로 삭제할때 두개로 분리되는것도 정수만 가능.
	사실 다른 타입은 테스트 안해봄.
	***********************************************/

	range_map<int, int> map;
	map.merge_insert({ 0, 15 }, 1);
	map.merge_insert({ 15, 30 }, 2);
	map.merge_insert({ 10, 20 }, 3);
	map.merge_insert({ 15 }, 4);

	for (auto& iter : map)
	{
		const range_map<int, int>::key_type& key = iter.first;
		const range_map<int, int>::value_type& value = iter.second;

		std::cout << key.begin() << " ~ " << key.end() << " = " << value << std::endl;
	}

	return 0;
}