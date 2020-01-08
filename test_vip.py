# demonstrate usage of Python variant wrappers
# usage (from build dir): PYTHONPATH=. ../test_vip.py

# grab stuff from the generated module
from vip import *

# variant parameter
doSomething(True)               # produces "got a b" (bool)
doSomething(42)                 # produces "got a i" (int)
doSomething(12.1)               # produces "got a d" (double)

# variant return type
print(produceSomething(True))   # 12.1
print(produceSomething(False))  # 'surprise!'

# wrapped custom class inside variant
print(foo(3.14))                # <vip.Foo object at 0x...>
print(foo(Foo(42, "hello")))    # 'hello variant'

# reference wrappers
bar(Foo(42, "hello"))           # const ref to Foo
bar(7)                          # regular parameter

# treatment of std::monostate and None
print(baz(12))                  # should be an int
print(baz(None))                # should be None
# bar(None)                     # should be error - bar requires a value

