#pragma once

struct Predicate
{
    const char* name;
    bool (*check)(int);
};

// на згенерованих даних обидва предикати завжди хибні (усі значення >= 0)
// тут none_of перевірить кожен елемент, найгірший випадок, мє

// одне порівняння на елемент, простий варіантік
bool isNegative(int x);

// двічі sin + cos на елемент, дорогий отут, а ще там завжди фолз тому ще гірше
bool trigAboveTwo(int x);

const Predicate CHEAP = {"cheap (x < 0)", isNegative};
const Predicate HEAVY = {"heavy (sin/cos > 2)", trigAboveTwo};
