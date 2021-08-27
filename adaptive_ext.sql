CREATE FUNCTION hello_world()
    RETURNS text
    AS 'MODULE_PATHNAME'
    LANGUAGE C;

CREATE OR REPLACE function hello_world() AS
'/home/cc/repo/adaptive_ext/adaptive_ext.so', 'hello_world'
LANGUAGE C;
