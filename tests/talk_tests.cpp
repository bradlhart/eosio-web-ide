#include <boost/test/unit_test.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <eosio/chain/permission_object.hpp>
#include <eosio/chain/resource_limits.hpp>
#include <eosio/testing/tester.hpp>

using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;

BOOST_AUTO_TEST_SUITE(talk_tests)

BOOST_AUTO_TEST_CASE(post) try {
    using namespace eosio::chain::literals;
    tester t{setup_policy::none};

    // Load contract
    t.create_account("talk"_n);
    t.set_code("talk"_n, read_wasm("talk.wasm"));
    t.set_abi("talk"_n, read_abi("talk.abi").data());
    t.produce_block();

    // Create users
    t.create_account("john"_n);
    t.create_account("jane"_n);
    // Test "post" action
    t.push_action(
        "talk"_n, "post"_n, "john"_n,
        mutable_variant_object //
        ("id", 1)              //
        ("reply_to", 0)        //
        ("user", "john")       //
        ("content", "post 1")  //
    );
    t.push_action(
        "talk"_n, "post"_n, "jane"_n,
        mutable_variant_object //
        ("id", 2)              //
        ("reply_to", 0)        //
        ("user", "jane")       //
        ("content", "post 2")  //
    );
    t.push_action(
        "talk"_n, "post"_n, "john"_n,
        mutable_variant_object       //
        ("id", 3)                    //
        ("reply_to", 2)              //
        ("user", "john")             //
        ("content", "post 3: reply") //
    );

    // Can't reply to non-existing message
    BOOST_CHECK_THROW(
        [&] {
            t.push_action(
                "talk"_n, "post"_n, "john"_n,
                mutable_variant_object       //
                ("id", 4)                    //
                ("reply_to", 99)             //
                ("user", "john")             //
                ("content", "post 3: reply") //
            );
        }(),
        fc::exception);
}

FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
