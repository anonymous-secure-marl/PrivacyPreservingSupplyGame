inline Bit::Bit(bool b, int party) {
	if (party == PUBLIC)
		bit = circ_exec->public_label(b);
	else prot_exec->feed(&bit, party, &b, 1); 
}

inline Bit Bit::select(const Bit & select, const Bit & new_v) const{
	Bit tmp = *this;
	tmp = tmp ^ new_v;
	tmp = tmp & select;
	return *this ^ tmp;
}

template<typename O>
inline O Bit::reveal(int party) const {
	O res;
	prot_exec->reveal(&res, party, &bit, 1);
	return res;
}

template<>
inline string Bit::reveal<string>(int party) const {
	bool res;
	prot_exec->reveal(&res, party, &bit, 1);
	return res ? "true" : "false";
}

inline Bit Bit::operator==(const Bit& rhs) const {
	return !(*this ^ rhs);
}

inline Bit Bit::operator!=(const Bit& rhs) const {
	return (*this) ^ rhs;
}

inline Bit Bit::operator &(const Bit& rhs) const{
	Bit res;
	res.bit = circ_exec->and_gate(bit, rhs.bit);
	return res;
}
inline Bit Bit::operator ^(const Bit& rhs) const{
	Bit res;
	res.bit = circ_exec->xor_gate(bit, rhs.bit);
	return res;
}

inline Bit Bit::operator |(const Bit& rhs) const{
	return (*this ^ rhs) ^ (*this & rhs);
}

inline Bit Bit::operator!() const {
	return circ_exec->not_gate(bit);
}
