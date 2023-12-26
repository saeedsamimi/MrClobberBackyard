#pragma once
char hasFlag(const short int flags, const short int flag) {
	return (flags & flag) == flag;
}

void addFlag(short int* flags, const short int flag) {
	*flags |= flag;
}

void removeFlag(short int* flags, const short int flag) {
	if (hasFlag(*flags, flag)) *flags ^= flag;
}
