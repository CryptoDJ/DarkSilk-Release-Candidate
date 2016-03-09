include Makefile.include
AM_CPPFLAGS =  $(INCLUDES) -I$(top_builddir)/src/obj \
  -I$(top_srcdir)/src/leveldb/include -I$(top_srcdir)/src/leveldb/helpers \
  -I$(builddir) $(BOOST_INCLUDES) $(BDB_CPPFLAGS)
AM_LDFLAGS = $(PTHREAD_CFLAGS)

noinst_LIBRARIES = libbitcoin.a

<<<<<<< HEAD
bin_PROGRAMS = darksilkd darksilk-cli
=======
if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

DARKSILK_CONFIG_INCLUDES=-I$(builddir)/config
DARKSILK_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

DARKSILK_INCLUDES += -I$(srcdir)/secp256k1/include

LIBDARKSILK_SERVER=libbitcoin_server.a
LIBDARKSILK_WALLET=libbitcoin_wallet.a
LIBDARKSILK_COMMON=libbitcoin_common.a
LIBDARKSILK_CLI=libbitcoin_cli.a
LIBDARKSILK_UTIL=libbitcoin_util.a
LIBDARKSILK_CRYPTO=crypto/libbitcoin_crypto.a
LIBDARKSILK_UNIVALUE=univalue/libbitcoin_univalue.a
LIBDARKSILKQT=qt/libbitcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libbitcoin_crypto.a \
  libbitcoin_util.a \
  libbitcoin_common.a \
  univalue/libbitcoin_univalue.a \
  libbitcoin_server.a \
  libbitcoin_cli.a
if ENABLE_WALLET
DARKSILK_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libbitcoin_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libbitcoin_zmq.a
endif

if BUILD_DARKSILK_LIBS
lib_LTLIBRARIES = libbitcoinconsensus.la
LIBDARKSILK_CONSENSUS=libbitcoinconsensus.la
else
LIBDARKSILK_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =

if BUILD_DARKSILKD
  bin_PROGRAMS += darksilkd
endif

if BUILD_DARKSILK_UTILS
  bin_PROGRAMS += darksilk-cli darksilk-tx
endif
>>>>>>> e6a14b6... Add ZeroMQ support. Notify blocks and transactions via ZeroMQ

SUBDIRS = . $(BUILD_QT) $(BUILD_TEST)
DIST_SUBDIRS = . qt test
.PHONY: FORCE
# darksilk core #
<<<<<<< HEAD
DARKSILK_CORE_H = addrman.h alert.h allocators.h base58.h bignum.h \
  darksilkrpc.h bloom.h chainparams.h checkpoints.h checkqueue.h \
  clientversion.h compat.h core.h crypter.h db.h hash.h init.h \
  key.h keystore.h leveldb.h limitedmap.h main.h miner.h mruset.h \
  netbase.h net.h protocol.h script.h serialize.h sync.h threadsafety.h \
  txdb.h ui_interface.h uint256.h util.h version.h walletdb.h wallet.h

JSON_H = json/json_spirit.h json/json_spirit_error_position.h \
  json/json_spirit_reader.h json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h json/json_spirit_utils.h \
  json/json_spirit_value.h json/json_spirit_writer.h \
  json/json_spirit_writer_template.h
=======
DARKSILK_CORE_H = \
  addrman.h \
  alert.h \
  amount.h \
  arith_uint256.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  net.h \
  netbase.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  pow.h \
  primitives/block.h \
  primitives/transaction.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  serialize.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

>>>>>>> e6a14b6... Add ZeroMQ support. Notify blocks and transactions via ZeroMQ

obj/build.h: FORCE
	@$(MKDIR_P) $(abs_top_builddir)/src/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
<<<<<<< HEAD
version.o: obj/build.h

libbitcoin_a_SOURCES = addrman.cpp alert.cpp allocators.cpp darksilkrpc.cpp bloom.cpp \
  chainparams.cpp checkpoints.cpp core.cpp crypter.cpp db.cpp hash.cpp \
  init.cpp key.cpp keystore.cpp leveldb.cpp main.cpp miner.cpp \
  netbase.cpp net.cpp noui.cpp protocol.cpp rpcblockchain.cpp rpcdump.cpp \
  rpcmining.cpp rpcnet.cpp rpcrawtransaction.cpp rpcwallet.cpp script.cpp \
  sync.cpp txdb.cpp util.cpp version.cpp wallet.cpp walletdb.cpp $(JSON_H) \
=======
libbitcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between darksilkd and darksilk-qt
libbitcoin_server_a_CPPFLAGS = $(DARKSILK_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbitcoin_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(DARKSILK_CORE_H)

if ENABLE_ZMQ
LIBDARKSILK_ZMQ=libbitcoin_zmq.a

libbitcoin_zmq_a_CPPFLAGS = $(DARKSILK_INCLUDES)
libbitcoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between darksilkd and darksilk-qt, but only linked
# when wallet enabled
libbitcoin_wallet_a_CPPFLAGS = $(DARKSILK_INCLUDES)
libbitcoin_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  $(DARKSILK_CORE_H)

# crypto primitives library
crypto_libbitcoin_crypto_a_CPPFLAGS = $(DARKSILK_CONFIG_INCLUDES)
crypto_libbitcoin_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# univalue JSON library
univalue_libbitcoin_univalue_a_SOURCES = \
  univalue/univalue.cpp \
  univalue/univalue.h \
  univalue/univalue_escapes.h \
  univalue/univalue_read.cpp \
  univalue/univalue_write.cpp

# common: shared between darksilkd, and darksilk-qt and non-server tools
libbitcoin_common_a_CPPFLAGS = $(DARKSILK_INCLUDES)
libbitcoin_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(DARKSILK_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbitcoin_util_a_CPPFLAGS = $(DARKSILK_INCLUDES)
libbitcoin_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpcprotocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(DARKSILK_CORE_H)

if GLIBC_BACK_COMPAT
libbitcoin_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between darksilk-cli and darksilk-qt
libbitcoin_cli_a_CPPFLAGS = $(NEXUS_INCLUDES)
libbitcoin_cli_a_SOURCES = \
  rpcclient.cpp \
>>>>>>> e6a14b6... Add ZeroMQ support. Notify blocks and transactions via ZeroMQ
  $(DARKSILK_CORE_H)

nodist_libbitcoin_a_SOURCES = $(top_srcdir)/src/obj/build.h
#

# darksilkd binary #
darksilkd_LDADD = libbitcoin.a leveldb/libleveldb.a leveldb/libmemenv.a \
  $(BOOST_LIBS)
darksilkd_SOURCES = darksilkd.cpp
#

if TARGET_WINDOWS
darksilkd_SOURCES += darksilkd-res.rc
endif

<<<<<<< HEAD
AM_CPPFLAGS += $(BDB_CPPFLAGS)
darksilkd_LDADD += $(BDB_LIBS)
=======
darksilkd_LDADD = \
  $(LIBDARKSILK_SERVER) \
  $(LIBDARKSILK_COMMON) \
  $(LIBDARKSILK_UNIVALUE) \
  $(LIBDARKSILK_UTIL) \
  $(LIBDARKSILK_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
darksilkd_LDADD += $(LIBDARKSILK_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
darksilkd_LDADD += libbitcoin_wallet.a
endif

darksilkd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)
>>>>>>> e6a14b6... Add ZeroMQ support. Notify blocks and transactions via ZeroMQ

# darksilk-cli binary #
darksilk_cli_LDADD = libbitcoin.a leveldb/libleveldb.a leveldb/libmemenv.a \
  $(BOOST_LIBS)
darksilk_cli_SOURCES = darksilk-cli.cpp
#

if TARGET_WINDOWS
darksilk_cli_SOURCES += darksilk-cli-res.rc
endif
darksilk_cli_LDADD += $(BDB_LIBS)

leveldb/libleveldb.a: leveldb/libmemenv.a

leveldb/%.a:
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
	  OPT="$(CXXFLAGS) $(CPPFLAGS)"

qt/darksilkstrings.cpp: $(libbitcoin_a_SOURCES)
	@test -n $(XGETTEXT) || echo "xgettext is required for updating translations"
	@cd $(top_srcdir); XGETTEXT=$(XGETTEXT) share/qt/extract_strings_qt.py

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb Makefile.include

clean-local:
	-$(MAKE) -C leveldb clean
	rm -f leveldb/port/*.gcno leveldb/db/*.gcno leveldb/table/*.gcno leveldb/helpers/*.gcno
	rm -f leveldb/util/*.gcno leveldb/helpers/memenv/*.gcno
