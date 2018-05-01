# lakType
Runtime type checking for C++11

# Usage

Check if two types are the same: `typeof<t1>() == typeof<t2>()`

Check if a lakType is a t1 type: `typeof<t1>() == laktype1` or `laktype1 == typeof<t1>()`

Check if lakType t1 is the same type as lakType t2: `t1 == t2`

Check if lakType t1 is not the same time as lakType t2: `t1 != t2`