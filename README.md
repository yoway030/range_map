# range_map
map which use numeric range key

# code
range_map<int, int> map;
map.merge_insert({ 0, 15 }, 1);
map.merge_insert({ 15, 30 }, 2);
map.merge_insert({ 10, 20 }, 3);
map.merge_insert({ 15 }, 4);
 
# print out
0 ~ 9 = 1
10 ~ 14 = 3
15 ~ 15 = 4
16 ~ 20 = 3
21 ~ 30 = 2
