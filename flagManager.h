#pragma once
inline char hasFlag(const short int flags, const short int flag) {
	return (flags & flag) == flag;
}

inline void addFlag(short int* flags, const short int flag) {
	*flags |= flag;
}

inline void removeFlag(short int* flags, const short int flag) {
	if (hasFlag(*flags, flag)) *flags ^= flag;
}
