#ifndef STRUCTBEGIN
#define STRUCTBEGIN(CLASS) {
#define STRUCTVALUE(Type,Name)
#define STRUCTFUNCTION(Ref,Name,...)
#define STRUCTEND() }
template <typename CT, typename FT>
auto public2ThisFunc0(CT ct, FT ft)->
decltype((ct->*ft)())
{
	return (ct->*ft)();
}
template <typename CT, typename FT, typename PT>
auto public2ThisFunc1(CT ct, FT ft, PT t)->
decltype((ct->*ft)(std::get<0>(t)))
{
	return (ct->*ft)(std::get<0>(t));
}
template <typename CT, typename FT, typename PT>
auto public2ThisFunc2(CT ct, FT ft, PT t)->
decltype((ct->*ft)(std::get<0>(t), std::get<1>(t)))
{
	return (ct->*ft)(std::get<0>(t), std::get<1>(t));
}
template <typename CT, typename FT, typename PT>
auto public2ThisFunc3(CT ct, FT ft, PT t)->
decltype((ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t)))
{
	return (ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t));
}

template <typename CT, typename FT, typename PT>
auto public2ThisFunc4(CT ct, FT ft, PT t)->
decltype((ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t)))
{
	return (ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
}
template <typename CT, typename FT, typename PT>
auto public2ThisFunc5(CT ct, FT ft, PT t)->
decltype((ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t)))
{
	return (ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
}
template <typename CT, typename FT, typename PT>
auto public2ThisFunc6(CT ct, FT ft, PT t)->
decltype((ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t)))
{
	return (ct->*ft)(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t));
}
#endif
//#define CHECKSET(LTYPE,VALUE,LTYPE) if(lua_type()