# demonstrating use of wrapped code from blog post
# usage (from build dir): PYTHONPATH=. ../test_blog_example.py

from blog_example import adder

# we get to use native Python types here. std::variant is hidden.
print(adder("the answer is ", 42))
print(adder(13, " is my lucky number"))  # multiple instantiations in a natural way
print(adder("a monoid", " in the category of endofunctors"))
# the return type is also a variant and produces native Python types
print(type(adder(1, 2)))                 # an int this time
