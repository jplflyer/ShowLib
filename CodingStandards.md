# Coding Standards
In order to avoid mismatched code, I'm going to ask people to follow some coding standards. They aren't too bad.

I'm not perfect, and if you see me doing something in a poor manner, feel free to point it out. But if you don't like the amount of white space I use, tough.

Note that these are guidelines. Even the "rules" I use can have exceptions.

# C++ Version
We're currently compiling to C++14, but I think there's no reason I can't change that to C++17.

C++20 isn't supported on every platform I touch (as of November, 2020). Yes, I know it has some really cool things. Eventually, I'll get to them.

Note: I care about Ubuntu 18.04 LTS and Mac OS Catalina. Yes, I know Big Sur is out. I'm not in a hurry to upgrade, as it always takes me days to get my system productive for development again, and I have too many other things to do rather than upgrade to an OS version without a single exciting feature.

# Naming Standards
Yes, I know I follow uncommon naming standards, although there is other precedent (see Qt, for instance). I am a long-time Java programmer, and I believe the Java standards are offer clarity.

* All names are CamelCase, although underscores can be added for additional readability.
* Class names begin with an upper case letter.
* All method (function) names and all variables begin with a lower case letter.

For instance;

    class Foo {
    private:
        int someInt;

    public:
        Foo & setSomeInt(int value) { someInt = value; return *this; }
    }

Note also the use of the Builder pattern on setters, but that's not a big deal.

# File Naming Standards
Use .cpp and .h.

# Optional Braces
Consider this code:

    if (condition) {
        doSomething();
    }

Clearly, this can be written without the braces like this:

    if (condition)
        doSomething();

Optional braces are not optional. This is a common source of coding errors. Do it the first way, not the second.

Note that I do, very, very rarely, do this:

    if (condition) doSomething();

But as soon as the line is at all longer than that, do it the first way above. If doSomething() appears on the next line, I do not consider the braces optional.

# White Space
Be generous with white space. We all have huge screens and can afford some screen real estate. I have old eyes, and scrunched-together code is difficult for me to read. For instance, consider these two lines of code:

    return columns.findIf([=](const Column::Pointer&ptr){return ptr->getName()==colName;});
    return columns.findIf( [=](const Column::Pointer &ptr){ return ptr->getName() == colName; } );

It's the same line of code. The second is the first line with every extraneous space removed. And one is (at least for me) significantly easier to read than the other.

## Location of Braces
Generally speaking, I put open braces with the if, as you see in some of my examples. However, if the if-condition is too long, I'll do one of these.

    if (
      /* several complicated and hard to read clauses */
      )
    {
    }

The idea is that readability is enhanced when with an appropriate level of white space.

## Standard For-Lops
    for (int foo = 0; foo < length(); ++foo) {
    }

## Auto-Initialiation and Other Punctuation.
Please add white space when dealing with a ton of punctuation characters in a row. For instance, I had a bug in work code when I wrote this:

    std::string cName = string{"mc"} + foo.cName();

That code is perfectly reasonable, except it needed to be this:

    std::string cName = string{"mc-"} + foo.cName();

And to my old eyes inside my IDE, that was just too many punctuation characters in a row. The {} characters are pretty twisty already, and I flat out missed the fact I was missing the dash. I've since done two things to help:

* Increased the font size on my IDE from 12 to 14-point. It helps.
* Changed my habit around {} to add white space (sometimes).

    std::string cName = string{ "mc-" } + foo.cName();

I actually think this is a little ugly, but that's probably because I'm not used to it yet. It is, however, more readable.

# Use of auto
Avoid this:

    auto gleep = someMethodCall();

Yes, I know, some people think auto is the greatest thing since sliced bread. But now I have to go look at what someMethodCall() returns in order to have a clue what gleep is. That isn't helpful just because you didn't want to type something. We're using a strongly-typed language because that helps eliminate problems, and now we're stepping back from that? I'd really rather you didn't.

I will use auto when dealing with std::containers, especially their iterators.

     std::vector<Foo> myVec;
     for (auto it: myVec) {
         Foo & foo = *it;
     }

     auto bar = std::find_if(myVec.cbegin(), myVec.cend(), [](){...} );

In both these cases, any programmer with any C++ experience knows that foo and bar are both iterators. But in the earlier case, we have no clue what gleep is.

# Weird Things I Do

## std::vector<Foo>
Okay, I don't know if this is weird or not. For complex data types, I nearly always use:

    std::vector<std::shared_Ptr<Foo>> myVector;

Maybe this isn't necessary a lot of the time, especially if I do proper move semantics on all my objects. But often enough I'm going to be inserting into or removing from the list, and the last thing I want is to move a bunch of complex objects.

I welcome comments on this. In my searching, I haven't found many discussions on this. This may be a really weird habit with better solutions, and I welcome discussion.

## Common Typedefs I Do
    class Foo {
    public:
        typedef std::shared_ptr<Foo> Pointer;
        typedef std::vector<Pointer> Vector;
    };

    Foo::Vector myVector;

You'll see that pattern a lot in my code. I don't know if this is weird or not. Feel free to share your opinions. Basically, I think stuff like this is really hard to read:

    std::vector<std::shared_ptr<Foo>> myVector;
