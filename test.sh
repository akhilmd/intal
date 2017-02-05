#!/bin/bash

YEL='\033[0;33m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

test()
{
    echo -e -n '\033[?7l'
    echo -e -n "[  ${YEL}RUNNING${NC} ] : [$1] -> [$2]"
    if [ "$($1)" = "$2" ]
    then echo -e "\r[  ${GREEN}PASSED${NC}  ]"
    else echo -e "\r[  ${RED}FAILED${NC}  ]"
    fi
    echo -e -n '\033[?7h'
}

runall()
{
    make

    if [ "$1" = "-all" ]
    then
        echo -e "\nTesting __Intal_uIsGreater"

        test "./intal_demo -gt +12345 +0" "1"
        test "./intal_demo -gt +0 +12345" "0"
        test "./intal_demo -gt +12121 +112121" "0"
        test "./intal_demo -gt +12121 +2121" "1"
        test "./intal_demo -gt +13589 +0000000125554" "0"
        test "./intal_demo -gt +000000000001358889 +0125554" "1"

        echo -e "\nTesting __Intal_uadd"

        test "./intal_demo -ua +12345 +0" "+12345"
        test "./intal_demo -ua +0 +12345" "+12345"
        test "./intal_demo -ua +12121 +112121" "+124242"
        test "./intal_demo -ua +12121 +2121" "+14242"
        test "./intal_demo -ua +13589 +0000000125554" "+139143"
        test "./intal_demo -ua +000000000001358889 +0125554" "+1484443"

        echo -e "\nTesting __Intal_usubtract"

        test "./intal_demo -us +12345 +0" "+12345"
        test "./intal_demo -us +112121 +12121" "+100000"
        test "./intal_demo -us +12121 +2121" "+10000"
        test "./intal_demo -us +0000000125554  +13589" "+111965"
        test "./intal_demo -us +000000000001358889 +0125554" "+1233335"

        echo -e "\nTesting __Intal_karatsuba"

        test "./intal_demo -uk +149837619847561987456 +230975443576" "+34608810708662691554805747782656"
        test "./intal_demo -uk +12345678 +32165487" "+397104745215186"

        echo -e "\nTesting __Intal_divideNewtonRaphson"

        test "./intal_demo -nr +123456789012345678901234567890123456789012345678901234567890 +100000000000000000" "+1234567890123456789012345678901234567890123"
        test "./intal_demo -nr +99 +4" "+24"
        test "./intal_demo -nr +3417562394 +873" "+3914733"

        echo -e "\nTesting __Intal_divideRepeatedSubtraction"

        test "./intal_demo -rs +99 +4" "+24"
        test "./intal_demo -rs +3417562394 +873" "+3914733"
    fi

    echo -e "\nTesting Intal_add"

    test "./intal_demo -sa +149837619847561987456 +230975443576" "+149837620078537431032"
    test "./intal_demo -sa -149837619847561987456 -230975443576" "-149837620078537431032"
    test "./intal_demo -sa +149837619847561987456 -230975443576" "+149837619616586543880"
    test "./intal_demo -sa -149837619847561987456 +230975443576" "-149837619616586543880"

    echo -e "\nTesting Intal_subtract"

    test "./intal_demo -ss +149837619847561987456 +230975443576" "+149837619616586543880"
    test "./intal_demo -ss -149837619847561987456 -230975443576" "-149837619616586543880"
    test "./intal_demo -ss +149837619847561987456 -230975443576" "+149837620078537431032"
    test "./intal_demo -ss -149837619847561987456 +230975443576" "-149837620078537431032"

    echo -e "\nTesting Intal_multiply"

    test "./intal_demo -sm +149837619847561987456 +230975443576" "+34608810708662691554805747782656"
    test "./intal_demo -sm -149837619847561987456 -230975443576" "+34608810708662691554805747782656"
    test "./intal_demo -sm +12345678 -32165487" "-397104745215186"
    test "./intal_demo -sm +9000000000 -100000000000000000000" "-900000000000000000000000000000"
    test "./intal_demo -sm -123123131312313213213213213132131 +9874651312038473548743541354132132" "-1215797990155418936404793315971471470306747085042031552922948733292"

    echo -e "\nTesting Intal_divide"

    test "./intal_demo -sd +99 +4" "+24"
    test "./intal_demo -sd -99 +4" "-24"
    test "./intal_demo -sd +3417562394 +873" "+3914733"
    test "./intal_demo -sd -3417562394 -873" "+3914733"
    test "./intal_demo -sd +9234756314875 +0" "ERROR: divide by zero!"

    echo -e "\nTesting Intal_pow"

    test "./intal_demo -sp +99 +4" "+96059601"
    test "./intal_demo -sp -99 +3" "-970299"
}

runall $1 | less -r