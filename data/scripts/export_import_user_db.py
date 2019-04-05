import argparse
import os
import sqlite3


# Connect to original and custom dbs
def connect_db(action, orig_file, custom_file):

    # If exporting, delete custom file if it already exists
    if action == 'export':
        try:
            os.remove(custom_file)
            print('Deleted existing', custom_file)
        except:
            pass

    # Connect to original db as main schema
    conn = sqlite3.connect(orig_file)

    # Attach custom db as a different schema
    conn.execute('ATTACH DATABASE "{filename}" AS custom'.format(filename = custom_file))

    return conn


# Get cursor to all user tables
def get_user_tables(conn, target_schema):
    cursor = conn.cursor()
    cursor.execute(
        '''SELECT * FROM {target_schema}.sqlite_master
        WHERE type = 'table' AND name LIKE "user_%"'''.format(target_schema = target_schema)
    )

    return cursor


# Export user tables
def export_user_tables(conn, cursor):
    for row in cursor:
        conn.execute(
            '''CREATE TABLE custom.{tbl_name} AS
            SELECT * FROM {tbl_name}'''.format(tbl_name = row[2])
        )
    
    conn.commit()


# Import user tables
def import_user_tables(conn, cursor):
    for row in cursor:
        try:
            conn.execute(
                '''INSERT INTO {tbl_name}
                SELECT * FROM custom.{tbl_name}'''.format(tbl_name = row[2])
            )
        except:
            print('Failed to import', row[2], '-- skipping!')
    
    conn.commit()


def main(action, orig_file, custom_file):

    # Open database connections
    conn = connect_db(action, orig_file, custom_file)

    # Get all user tables
    cursor = get_user_tables(conn, 'main' if action == 'export' else 'custom')

    # Export or import user tables
    if action == 'export':
        export_user_tables(conn, cursor)
    else:
        import_user_tables(conn, cursor)
    
    conn.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Utility to export and import the custom user tables in the paperblossoms db via another sqlite file.')
    parser.add_argument('action', choices = ['export', 'import'], help = 'Which action you want to take, either "export" from or "import" into the paperblossoms db')
    parser.add_argument('orig_db', help = 'Filepath for original db, paperblossoms.db')
    parser.add_argument('custom_db', help = 'Filepath for exported db with custom user tables')
    args = parser.parse_args()

    main(args.action, args.orig_db, args.custom_db)
