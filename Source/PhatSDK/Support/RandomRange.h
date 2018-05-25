#pragma once

enum eRandomFormula
{
	favorSpecificValue,
	favorLow,
	favorMid,
	favorHigh,
	equalDistribution,
};


void testRandomValueGenerator();

int getRandomNumberWithFavoredValue(int minInclusive, int maxInclusive, double favorValue, double favorStrength);

int getRandomNumberExclusive(int maxExclusive);
int getRandomNumberExclusive(int maxExclusive, eRandomFormula formula, double favorStrength, double favorModifier, double favorSpecificValue = 0);

int getRandomNumber(int maxInclusive);
int getRandomNumber(int minInclusive, int maxInclusive);

int getRandomNumber(int minInclusive, int maxInclusive, eRandomFormula formula, double favorStrength, double favorModifier, double favorSpecificValue = 0);

std::set<int> getRandomNumbersNoRepeat(int amount, int minInclusive, int maxInclusive);
int getRandomNumberNoRepeat(int minInclusive, int maxInclusive, std::set<int> notThese, int maxTries = 10);

double getRandomDouble(double maxInclusive);
double getRandomDouble(double maxInclusive, eRandomFormula formula, double favorStrength, double favorModifier, double favorSpecificValue = 0);
double getRandomDouble(double minInclusive, double maxInclusive);
double getRandomDouble(double minInclusive, double maxInclusive, eRandomFormula formula, double favorStrength, double favorModifier, double favorSpecificValue = 0);
