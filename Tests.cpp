#ifndef AVLLAST_AVLTESTS_H
#define AVLLAST_AVLTESTS_H


TEST (Group1, AVLOrderTest1) {
    TreeLib::AVLMap<int, int> map;
    for (int i = 0; i < 1000; ++i) {
        map.Insert(i, i);
    }
    ASSERT_EQ(map.begin()->key, 0);
}

TEST (Group1, OperatorTest1) {
    TreeLib::AVLMap<int, int> map;
    for (int i = 0; i < 1000; ++i) {
        map[i] = i + 2;
    }
    ASSERT_EQ(map[78], 80);
}

TEST (Group1, GetTest1) {
    TreeLib::AVLMap<int, int> map;
    for (int i = 0; i < 1000; ++i) {
        map.Insert(i, i + 1);
    }
    std::cout << map.Get(89);
    ASSERT_EQ(map[89], 90);
}

TEST (Group1, CopyTest1) {
    TreeLib::AVLMap<int, int> map1;
    TreeLib::AVLMap<int, int> map2;
    for (int i = 0; i < 1000; ++i) {
        map1.Insert(i, i);
    }
    map2 = map1;
    ASSERT_EQ(map1[2], map2[2]);
}

TEST (Group2, SetTest) {
    std::string s = "qwertyuiopasdfghjklzxcvbnm";
    TreeLib::AVLSet<char> sorted;
    for (auto&&x : s) {
        sorted.Insert(x);
    }
    std::sort(s.begin(), s.end());
    auto s2 = sorted.ToString();
    s2.erase(std::remove(s2.begin(), s2.end(), ' '), s2.end());
    ASSERT_EQ(s2, s);
}

TEST (Group2, StringTest) {
    TreeLib::AVLMap<int, int> map;
    map.Insert(0,0);
    map.Insert(1,-1);
    map.Insert(2,-101);
    map.Insert(3,10);
    map.Insert(4,10);
    map.Insert(5,30);

    std::string s = map.ToString("RCL");
    std::cout << s << std::endl;
    auto tree = TreeLib::AVLMap<int, int>::FromString(s);
    std::cout << tree.ToString("RCL");
    ASSERT_EQ(map.begin()->key, tree.begin()->key);
}

TEST (GroupDelete, DeleteTest) {
    TreeLib::AVLMap<int, int> map;
    for (size_t i = 0; i < 1000; ++i) {
        map.Insert(i, 1);
    }
    map.Delete(10);
    ASSERT_FALSE(map.Contains(10));
}

/*
TEST (GroupIterator, IteratorTest) {
    std::vector<int> v = {1, 3, 4, 4, 1, 33, 345, 35, 2};
    std::vector<int> result;
    TreeLib::AVLSet<int> map;
    for (auto&& x : v){
        map.Insert(x);
    }

    for(int i = 0; i < v.size(); ++i) {
        result.push_back();
    }
     std::sort(v.begin(), v.end());

    ASSERT_TRUE(map.Contains(1));
}
 */


#endif //AVLLAST_AVLTESTS_H