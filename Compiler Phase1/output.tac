t0 = 30
_id1 = t0
t1 = 40
_id2 = t1
t2 = 50
_id3 = t2
L0:
t3 = 3
t4 = 2
t5 = t3 + t4
t6 = t5 * _id1
t7 = _id3 - _id1
t8 = t7 / _id2
t9 = _id2 + t8
t10 = t6 <> t9
if ( !t10 ) goto L1
t11 = _id2 <= _id3
if ( !t11 ) goto L2
_id2 = _id3
goto L3
L2:
_id3 = _id2
L3:
goto L0
L1:
