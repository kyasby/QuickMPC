# !!!!!!!!!! NOTE: 関数の内部処理はexit_statusのANDを取るために必ずワンライナーで記述する !!!!!!!!!!

# build処理を記述する関数
build() {
    git submodule update --init --recursive && \
    docker-compose -f docker-compose.yml build --build-arg BUILD_OPT="--config=debug" medium-libc dev_mc1 dev_mc2 dev_mc3 dev_mc_envoy3 dev_cc3 dev_dbgate3 dev_sharedb3 dev_secrets-server dev_mc_envoy2 dev_cc2 dev_dbgate2 dev_sharedb2 dev_mc_envoy1 dev_cc1 dev_dbgate1 dev_bts dev_sharedb1
}

# runの前に実行されるsetup処理を記述する関数
# INFO: runを冪等に実行できるように初期化を行う
setup() {
    docker-compose -f docker-compose.yml down -v
}

# run処理を記述する関数
# NOTE: この関数は例外的にワンライナーで書かなくて良い
run() {
    build
    docker-compose -f docker-compose.yml up medium-libc
}

# runの後に実行されるteardown処理を記述する関数
# INFO: runの後に副作用を残さないように初期化を行う
teardown() {
    docker-compose -f docker-compose.yml down -v
}
