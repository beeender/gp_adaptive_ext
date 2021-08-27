#include "postgres.h"
#include "dynloader.h"
#include "fmgr.h"
#include "utils/builtins.h"

//PG_MODULE_MAGIC;

extern const Pg_magic_struct *Pg_magic_func(void);

static bool is_gpdb6();

const Pg_magic_struct * Pg_magic_func(void) {
    static const Pg_magic_struct Pg_magic_data_6 = {
        sizeof(Pg_magic_struct), 60000 / 100, 100, 32, 64, ((bool) 1), ((bool) 1), 2180, 1, };
    static const Pg_magic_struct Pg_magic_data_5 = {
        sizeof(Pg_magic_struct), 50000 / 100, 100, 32, 64, 1, 1, 2180, 1, };
    if (is_gpdb6())
    {
        return &Pg_magic_data_6;
    }
    return &Pg_magic_data_5;
}

PG_FUNCTION_INFO_V1(hello_world);

void* handle;

Datum
hello_world(PG_FUNCTION_ARGS)
{
    PGFunction *func_ptr = pg_dlsym(handle, "hello_gpdb");
    (fcinfo)->flinfo->fn_addr = func_ptr;
    return FunctionCallInvoke(fcinfo);
}

static bool inited = false;
static bool gpdb6 = false;

bool is_gpdb6()
{
    if (!inited)
    {
        char *ver = TextDatumGetCString(pgsql_version(NULL));
        if (strstr(ver, "Greenplum Database 6")) {
            gpdb6 = true;
        }
        inited = true;
    }
    return gpdb6;
}

#if GP_VERSION_NUM == 60000
Datum
hello_gpdb(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text("Hello, GPDB6"));
}
#else
Datum
hello_gpdb(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text("Hello, GPDB5"));
}
#endif

void
_PG_init(void)
{
    elog(NOTICE, "_PG_init for adaptive_ext");
    if (is_gpdb6())
    {
        handle = pg_dlopen("/home/cc/repo/adaptive_ext/gpdb6.so");
    }
    else
    {
        handle = pg_dlopen("/home/cc/repo/adaptive_ext/gpdb5.so");
    }
}
