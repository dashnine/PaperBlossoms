import os
import sqlite3
import json


def connect_db(db_file):

    # Delete database file if it exists
    if os.path.exists(db_file):
        os.remove(db_file)

    # Create connection to new database file
    conn = sqlite3.connect(db_file)
    return conn


# Create base_{table_stem} and user_{table_stem} from create_stmt
# Define a {table_stem} view with description fields for desc_fields
# and translation fields for tr_fields
# tr_fields should be a list of strings
# desc_fields should be either a string or a dictionary of strings;
# if the latter, the key should be the name of the field to be described
# and the value should be the prefix of the description fields;
# if the former, no prefix is assumed to be present
def create_tables(db_conn, table_stem, create_stmt, desc_fields = None, tr_fields = None):

    # Set names of base and user tables, respectively
    base_table = 'base_' + table_stem
    user_table = 'user_' + table_stem

    # Create tables using the same statements
    db_conn.execute(create_stmt.format(base_table))
    db_conn.execute(create_stmt.format(user_table))

    # Dynamically create portions of view definition for translated fields
    tr_select = [
        ', COALESCE(i18n_{tr_field}.string_tr, t.{tr_field}) AS {tr_field}_tr'.format(tr_field = tr_field)
        for tr_field in tr_fields
    ] if tr_fields is not None else []
    
    tr_join = [
        'LEFT JOIN i18n i18n_{tr_field} ON t.{tr_field} = i18n_{tr_field}.string'.format(tr_field = tr_field)
        for tr_field in tr_fields
    ] if tr_fields is not None else []

    # Dynamically create portions of view definition for descriptions
    if type(desc_fields) == str:
        desc_fields = { desc_fields: '' }
    desc_select = [
        ', {field}_desc.description AS '.format(field = field) + (
            'description' if desc_fields[field] == '' else '_'.join([desc_fields[field], 'description'])
        ) +
        ', {field}_desc.short_desc AS '.format(field = field) + (
            'short_desc' if desc_fields[field] == '' else '_'.join([desc_fields[field], 'short_desc'])
        )
        for field in desc_fields
    ] if desc_fields is not None else []
    
    desc_join = [
        'LEFT JOIN user_descriptions {desc_field}_desc ON t.{desc_field} = {desc_field}_desc.name'.format(desc_field = desc_field)
        for desc_field in desc_fields
    ] if desc_fields is not None else []
    
    # Build view definition from combination of user and base tables, descriptions and translations
    view_defn = '\n'.join(
        ['CREATE VIEW {table_stem} AS'.format(table_stem = table_stem)] +
        ['SELECT t.*'] +
        desc_select + tr_select +
        ['''FROM (
            SELECT * FROM {base_table}
            UNION ALL
            SELECT * FROM {user_table}
        ) t'''.format(base_table = base_table, user_table = user_table)] +
        desc_join + tr_join
    )
    
    # Create view in db
    db_conn.execute(view_defn)


def rings_to_db(db_conn):

    # Create rings table
    create_tables(
        db_conn,
        'rings',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            outstanding_quality TEXT
        )''',
        tr_fields = ['name', 'outstanding_quality']
    )

    # Read rings JSON
    with open('json/rings.json', encoding = 'utf8') as f:
        rings = json.load(f)

    # Write rings to rings table
    for ring in rings:
        db_conn.execute(
            'INSERT INTO base_rings VALUES (?,?)',
            (ring['name'], ring['outstanding_quality'])
        )


def skills_to_db(db_conn):

    # Create skills table
    create_tables(
        db_conn,
        'skills',
        '''CREATE TABLE {} (
            skill_group TEXT,
            skill TEXT PRIMARY KEY
        )''',
        tr_fields = ['skill_group', 'skill']
    )

    # Read skills JSON
    with open('json/skill_groups.json', encoding = 'utf8') as f:
        skill_groups = json.load(f)

    # Write skills to skills table
    for skill_group in skill_groups:
        for skill in skill_group['skills']:
            db_conn.execute(
                'INSERT INTO base_skills VALUES (?,?)',
                (skill_group['name'], skill)
            )


def qualities_to_db(db_conn):

    # Create qualities table
    create_tables(
        db_conn,
        'qualities',
        '''CREATE TABLE {} (
            quality TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER
        )''',
        desc_fields = 'quality',
        tr_fields = ['quality']
    )

    # Read qualities JSON
    with open('json/qualities.json', encoding = 'utf8') as f:
        qualities = json.load(f)

    # Write qualities to qualities table
    for quality in qualities:
        db_conn.execute(
            'INSERT INTO base_qualities VALUES (?,?,?)',
            (
                quality['name'],
                quality['reference']['book'],
                quality['reference']['page']
            )
        )


def personal_effects_to_db(db_conn):

    # Create personal effects table
    create_tables(
        db_conn,
        'personal_effects',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            price_value INTEGER,
            price_unit TEXT,
            rarity INTEGER
        )''',
        desc_fields = 'name',
        tr_fields = ['name']
    )

    # Create personal effects qualities table
    create_tables(
        db_conn,
        'personal_effect_qualities',
        '''CREATE TABLE {} (
            personal_effect TEXT,
            quality TEXT
        )''',
        tr_fields = ['personal_effect', 'quality']
    )

    # Read personal effects JSON
    with open('json/personal_effects.json', encoding = 'utf8') as f:
        personal_effects = json.load(f)

    # Write personal effects to personal effects tables
    for item in personal_effects:

        # Write personal effects to personal effects table
        db_conn.execute(
            'INSERT INTO base_personal_effects VALUES (?,?,?,?,?,?)',
            (
                item['name'],
                item['reference']['book'],
                item['reference']['page'],
                item['price']['value'] if 'price' in item else None,
                item['price']['unit'] if 'price' in item else None,
                item['rarity'] if 'rarity' in item else None
            )
        )

        # Write personal effect qualities
        if 'qualities' in item:
            db_conn.executemany(
                'INSERT INTO base_personal_effect_qualities VALUES (?,?)',
                [
                    (item['name'], quality)
                    for quality in item['qualities']
                ]
            )


def armor_to_db(db_conn):

    # Create armor table
    create_tables(
        db_conn,
        'armor',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            rarity INTEGER,
            price_value INTEGER,
            price_unit TEXT
        )''',
        desc_fields = 'name',
        tr_fields = ['name']
    )
    # Create resistance values table
    create_tables(
        db_conn,
        'armor_resistance',
        '''CREATE TABLE {} (
            armor TEXT,
            resistance_category TEXT,
            resistance_value INTEGER
        )''',
        tr_fields = ['armor']
    )
    # Create qualities table
    create_tables(
        db_conn,
        'armor_qualities',
        '''CREATE TABLE {} (
            armor TEXT,
            quality TEXT
        )''',
        tr_fields = ['armor', 'quality']
    )

    # Read armor JSON
    with open('json/armor.json', encoding = 'utf8') as f:
        armor = json.load(f)

    # Write armor to armor, resistance values and qualities tables
    for piece in armor:

        # Write to armor table
        db_conn.execute(
            'INSERT INTO base_armor VALUES (?,?,?,?,?,?)',
            (
                piece['name'],
                piece['reference']['book'],
                piece['reference']['page'],
                piece['rarity'],
                piece['price']['value'],
                piece['price']['unit']
            )
        )
        # Write to resistance values table
        for resistance_value in piece['resistance_values']:
            db_conn.execute(
                'INSERT INTO base_armor_resistance VALUES (?,?,?)',
                (
                    piece['name'],
                    resistance_value['category'],
                    resistance_value['value']
                )
            )
        # Write to qualities table
        for quality in piece['qualities']:
            db_conn.execute(
                'INSERT INTO base_armor_qualities VALUES (?,?)',
                (piece['name'], quality)
            )


def weapons_to_db(db_conn):

    # Create weapons table
    create_tables(
        db_conn,
        'weapons',
        '''CREATE TABLE {} (
            category TEXT,
            name TEXT,
            reference_book TEXT,
            reference_page INTEGER,
            skill TEXT,
            grip TEXT,
            range_min INTEGER,
            range_max INTEGER,
            damage INTEGER,
            deadliness INTEGER,
            rarity INTEGER,
            price_value INTEGER,
            price_unit TEXT,
            PRIMARY KEY (name, grip)
        )''',
        desc_fields = 'name',
        tr_fields = ['category', 'name', 'skill', 'grip']
    )
    # Create qualities table
    create_tables(
        db_conn,
        'weapon_qualities',
        '''CREATE TABLE {} (
            weapon TEXT,
            grip TEXT,
            quality TEXT
        )''',
        tr_fields = ['weapon', 'quality']
    )

    # Read weapons JSON
    with open('json/weapons.json', encoding = 'utf8') as f:
        weapon_categories = json.load(f)

    # Write weapons to weapons and qualities tables
    for category in weapon_categories:
        for weapon in category['entries']:
            for grip in weapon['grips']:

                # Write to weapons table
                db_conn.execute(
                    'INSERT INTO base_weapons VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)',
                    (
                        category['name'],
                        weapon['name'],
                        weapon['reference']['book'],
                        weapon['reference']['page'],
                        (
                            weapon['skill']
                            if not any([
                                effect['attribute'] == 'skill'
                                for effect in grip['effects']
                            ])
                            else [
                                effect['value']
                                for effect in grip['effects']
                                if effect['attribute'] == 'skill'
                            ].pop()
                        ),
                        grip['name'],
                        (
                            weapon['range']['min']
                            if not any([
                                effect['attribute'] == 'range'
                                for effect in grip['effects']
                            ])
                            else [
                                effect['value']['min']
                                for effect in grip['effects']
                                if effect['attribute'] == 'range'
                            ].pop()
                        ),
                        (
                            weapon['range']['max']
                            if not any([
                                effect['attribute'] == 'range'
                                for effect in grip['effects']
                            ])
                            else [
                                effect['value']['max']
                                for effect in grip['effects']
                                if effect['attribute'] == 'range'
                            ].pop()
                        ),
                        (
                            weapon['damage']
                            if not any([
                                effect['attribute'] == 'damage'
                                for effect in grip['effects']
                            ])
                            else [
                                weapon['damage'] + effect['value_increase']
                                for effect in grip['effects']
                                if effect['attribute'] == 'damage'
                            ].pop()
                        ),
                        (
                            weapon['deadliness']
                            if not any([
                                effect['attribute'] == 'deadliness'
                                for effect in grip['effects']
                            ])
                            else [
                                weapon['deadliness'] + effect['value_increase']
                                for effect in grip['effects']
                                if effect['attribute'] == 'deadliness'
                            ].pop()
                        ),
                        weapon['rarity'],
                        weapon['price']['value'],
                        weapon['price']['unit']
                    )
                )

                # Write grip effect to qualities table
                if any([effect['attribute'] == 'quality' for effect in grip['effects']]):
                    db_conn.executemany(
                        'INSERT INTO base_weapon_qualities VALUES (?,?,?)',
                        [
                            (weapon['name'], grip['name'], effect['value'])
                            for effect in grip['effects']
                            if effect['attribute'] == 'quality'
                        ]
                    )

            # Write to qualities table
            db_conn.executemany(
                'INSERT INTO base_weapon_qualities VALUES (?,?,?)',
                [
                    (weapon['name'], None, quality)
                    for quality in weapon['qualities']
                ]
            )


def techniques_to_db(db_conn):

    # Create techniques table
    create_tables(
        db_conn,
        'techniques',
        '''CREATE TABLE {} (
            category TEXT,
            subcategory TEXT,
            name TEXT PRIMARY KEY,
            restriction TEXT,
            reference_book TEXT,
            reference_page INTEGER,
            rank INTEGER,
            xp INTEGER
        )''',
        desc_fields = 'name',
        tr_fields = ['category', 'subcategory', 'name', 'restriction']
    )

    # Read techniques JSON
    with open('json/techniques.json', encoding = 'utf8') as f:
        technique_categories = json.load(f)

    # Write techniques to techniques table
    for category in technique_categories:
        for subcategory in category['subcategories']:
            for technique in subcategory['techniques']:
                db_conn.execute(
                    'INSERT INTO base_techniques VALUES (?,?,?,?,?,?,?,?)',
                    (
                        category['name'],
                        subcategory['name'],
                        technique['name'],
                        technique['restriction'] if 'restriction' in technique else None,
                        technique['reference']['book'],
                        technique['reference']['page'],
                        technique['rank'],
                        technique['xp']
                    )
                )


def advantages_to_db(db_conn):

    # Create advantages table
    create_tables(
        db_conn,
        'advantages_disadvantages',
        '''CREATE TABLE {} (
            category TEXT,
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            ring TEXT,
            types TEXT,
            effects TEXT
        )''',
        desc_fields = 'name',
        tr_fields = ['name', 'ring', 'types']
    )

    # Read advantages JSON
    with open('json/advantages_disadvantages.json', encoding = 'utf8') as f:
        advantage_categories = json.load(f)

    # Write advantages to advantages table
    for category in advantage_categories:
        for entry in category['entries']:
            db_conn.execute(
                'INSERT INTO base_advantages_disadvantages VALUES (?,?,?,?,?,?,?)',
                (
                    category['name'],
                    entry['name'],
                    entry['reference']['book'],
                    entry['reference']['page'],
                    entry['ring'],
                    ', '.join(entry['types']),
                    entry['effects']
                )
            )


def q8_to_db(db_conn):

    # Create question 8 table
    db_conn.execute(
        '''CREATE TABLE base_unorthodox_skills (
            skill TEXT PRIMARY KEY
        )'''
    )

    # Read question 8 JSON
    with open('json/question_8.json', encoding = 'utf8') as f:
        question_8 = json.load(f)

    # Write question 8 to table
    db_conn.executemany(
        'INSERT INTO base_unorthodox_skills VALUES (?)',
        [(skill,) for skill in question_8[1]['outcome']['values']]
    )


def clans_to_db(db_conn):

    # Create clans, families, family rings and family skills tables
    create_tables(
        db_conn,
        'clans',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            type TEXT,
            ring TEXT,
            skill TEXT,
            status INTEGER
        )''',
        desc_fields = 'name',
        tr_fields = ['name', 'type', 'ring', 'skill']
    )
    create_tables(
        db_conn,
        'families',
        '''CREATE TABLE {} (
            clan TEXT,
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            glory TEXT,
            wealth TEXT
        )''',
        desc_fields = 'name',
        tr_fields = ['clan', 'name']
    )
    create_tables(
        db_conn,
        'family_rings',
        '''CREATE TABLE {} (
            family TEXT,
            ring TEXT
        )''',
        tr_fields = ['family', 'ring']
    )
    create_tables(
        db_conn,
        'family_skills',
        '''CREATE TABLE {} (
            family TEXT,
            skill TEXT
        )''',
        tr_fields = ['family', 'skill']
    )

    # Read clans JSON
    with open('json/clans.json', encoding = 'utf8') as f:
        clans = json.load(f)

    # Write to tables
    for clan in clans:

        # Write to clans table
        db_conn.execute(
            'INSERT INTO base_clans VALUES (?,?,?,?,?,?,?)',
            (
                clan['name'],
                clan['reference']['book'],
                clan['reference']['page'],
                clan['type'],
                clan['ring_increase'],
                clan['skill_increase'],
                clan['status']
            )
        )

        for family in clan['families']:

            # Write to families table
            db_conn.execute(
                'INSERT INTO base_families VALUES (?,?,?,?,?,?)',
                (
                    clan['name'],
                    family['name'],
                    family['reference']['book'],
                    family['reference']['page'],
                    family['glory'],
                    family['wealth']
                )
            )

            # Write to family rings table
            db_conn.executemany(
                'INSERT INTO base_family_rings VALUES (?,?)',
                [
                    (family['name'], ring)
                    for ring in family['ring_increase']
                ]
            )

            # Write to family skills table
            db_conn.executemany(
                'INSERT INTO base_family_skills VALUES (?,?)',
                [
                    (family['name'], skill)
                    for skill in family['skill_increase']
                ]
            )


def heritage_to_db(db_conn):

    # Create heritage, heritage modifier, heritage effects table
    create_tables(
        db_conn,
        'samurai_heritage',
        '''CREATE TABLE {} (
            source TEXT,
            roll_min INTEGER,
            roll_max INTEGER,
            ancestor TEXT PRIMARY KEY,
            modifier_glory INTEGER,
            modifier_honor INTEGER,
            modifier_status INTEGER,
            effect_type TEXT,
            effect_instructions TEXT
        )''',
        tr_fields = ['ancestor', 'effect_type', 'effect_instructions']
    )
    create_tables(
        db_conn,
        'heritage_effects',
        '''CREATE TABLE {} (
            ancestor TEXT,
            roll_min INTEGER,
            roll_max INTEGER,
            outcome TEXT
        )''',
        tr_fields = ['ancestor', 'outcome']
    )

    # Read samurai heritage from JSON
    with open('json/samurai_heritage.json', encoding = 'utf8') as f:
        samurai_heritage = json.load(f)

    # Write to heritage tables
    for ancestor in samurai_heritage:

        # Write to samurai heritage table
        db_conn.execute(
            'INSERT INTO base_samurai_heritage VALUES (?,?,?,?,?,?,?,?,?)',
            (
                ancestor['source'],
                ancestor['roll']['min'],
                ancestor['roll']['max'],
                ancestor['result'],
                ancestor['modifiers']['glory'],
                ancestor['modifiers']['honor'],
                ancestor['modifiers']['status'],
                ancestor['other_effects']['type'],
                ancestor['other_effects']['instructions']
            )
        )

        # Write to heritage effects table
        if 'outcomes' in ancestor['other_effects']:
            db_conn.executemany(
                'INSERT INTO base_heritage_effects VALUES (?,?,?,?)',
                [
                    (
                        ancestor['result'],
                        effect['roll']['min'] if 'roll' in effect else None,
                        effect['roll']['max'] if 'roll' in effect else None,
                        effect['outcome']
                    )
                    for effect in ancestor['other_effects']['outcomes']
                ]
            )


def schools_to_db(db_conn):

    # Create school tables
    create_tables(
        db_conn,
        'schools',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            role TEXT,
            clan TEXT,
            starting_skills_size INTEGER,
            honor INTEGER,
            advantage_disadvantage TEXT,
            school_ability_name TEXT,
            mastery_ability_name TEXT
        )''',
        desc_fields = {
            'name': '',
            'school_ability_name': 'school_ability',
            'mastery_ability_name': 'mastery_ability'
        },
        tr_fields = ['name', 'role', 'clan', 'school_ability_name', 'mastery_ability_name']
    )
    create_tables(
        db_conn,
        'school_rings',
        '''CREATE TABLE {} (
            school TEXT,
            ring TEXT
        )''',
        tr_fields = ['school', 'ring']
    )
    create_tables(
        db_conn,
        'school_starting_skills',
        '''CREATE TABLE {} (
            school TEXT,
            skill TEXT
        )''',
        tr_fields = ['school', 'skill']
    )
    create_tables(
        db_conn,
        'school_techniques_available',
        '''CREATE TABLE {} (
            school TEXT,
            technique TEXT
        )''',
        tr_fields = ['school', 'technique']
    )
    create_tables(
        db_conn,
        'school_starting_techniques',
        '''CREATE TABLE {} (
            school TEXT,
            set_id INTEGER,
            set_size INTEGER,
            technique TEXT
        )''',
        tr_fields = ['school', 'technique']
    )
    create_tables(
        db_conn,
        'school_starting_outfit',
        '''CREATE TABLE {} (
            school TEXT,
            set_id INTEGER,
            set_size INTEGER,
            equipment TEXT
        )''',
        tr_fields = ['school', 'equipment']
    )
    create_tables(
        db_conn,
        'curriculum',
        '''CREATE TABLE {} (
            school TEXT,
            rank INTEGER,
            advance TEXT,
            type TEXT,
            special_access INTEGER,
            min_allowable_rank INTEGER,
            max_allowable_rank INTEGER
        )''',
        tr_fields = ['school', 'advance']
    )

    # Read schools JSON
    with open('json/schools.json', encoding = 'utf8') as f:
        schools = json.load(f)

    # Write to schools tables
    for school in schools:

        # Write to schools table
        db_conn.execute(
            'INSERT INTO base_schools VALUES (?,?,?,?,?,?,?,?,?,?)',
            (
                school['name'],
                school['reference']['book'],
                school['reference']['page'],
                ', '.join(school['role']),
                school['clan'] if 'clan' in school else None,
                school['starting_skills']['size'],
                school['honor'],
                school['advantage_disadvantage'] if 'advantage_disadvantage' in school else None,
                school['school_ability'],
                school['mastery_ability'],
            )
        )

        # Write to school rings table
        db_conn.executemany(
            'INSERT INTO base_school_rings VALUES (?,?)',
            [
                (school['name'], ring)
                for ring in school['ring_increase']
            ]
        )

        # Write to school starting skill table
        db_conn.executemany(
            'INSERT INTO base_school_starting_skills VALUES (?,?)',
            [
                (school['name'], skill)
                for skill in school['starting_skills']['set']
            ]
        )

        # Write to school techniques available table
        db_conn.executemany(
            'INSERT INTO base_school_techniques_available VALUES (?,?)',
            [
                (school['name'], technique)
                for technique in school['techniques_available']
            ]
        )

        # Write to school starting techniques table
        for technique_set_id, technique_set in enumerate(school['starting_techniques']):
            db_conn.executemany(
                'INSERT INTO base_school_starting_techniques VALUES (?,?,?,?)',
                [
                    (
                        school['name'],
                        technique_set_id,
                        technique_set['size'],
                        technique
                    )
                    for technique in technique_set['set']
                ]
            )

        # Write to schools starting outfit table
        for equipment_set_id, equipment_set in enumerate(school['starting_outfit']):
            db_conn.executemany(
                'INSERT INTO base_school_starting_outfit VALUES (?,?,?,?)',
                [
                    (
                        school['name'],
                        equipment_set_id,
                        equipment_set['size'],
                        piece
                    )
                    for piece in equipment_set['set']
                ]
            )

        # Write to curriculum table
        db_conn.executemany(
            'INSERT INTO base_curriculum VALUES (?,?,?,?,?,?,?)',
            [
                (
                    school['name'],
                    advancement['rank'],
                    advancement['advance'],
                    advancement['type'],
                    int(advancement['special_access']),
                    advancement['allowable_rank']['min'] if 'allowable_rank' in advancement else None,
                    advancement['allowable_rank']['max'] if 'allowable_rank' in advancement else None
                )
                for advancement in school['curriculum']
            ]
        )


def titles_to_db(db_conn):

    # Create titles table
    create_tables(
        db_conn,
        'titles',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            xp_to_completion INTEGER,
            title_ability_name TEXT
        )''',
        desc_fields = {
            'name': '',
            'title_ability_name': 'title_ability'
        },
        tr_fields = ['name', 'title_ability_name']
    )

    # Create awards table for titles
    create_tables(
        db_conn,
        'title_awards',
        '''CREATE TABLE {} (
            title TEXT,
            social_attribute TEXT,
            base_award INTEGER,
            constraint_type TEXT,
            constraint_value INTEGER,
            constraint_min INTEGER,
            constraint_max INTEGER
        )''',
        tr_fields = ['title']
    )

    # Create advancement table for titles
    create_tables(
        db_conn,
        'title_advancements',
        '''CREATE TABLE {} (
            title TEXT,
            rank INTEGER,
            name TEXT,
            type TEXT,
            special_access INTEGER
        )''',
        tr_fields = ['title', 'name', 'type']
    )

    # Read titles from JSON
    with open('json/titles.json', encoding = 'utf8') as f:
        titles = json.load(f)

    # Write to titles tables
    for title in titles:

        # Write to titles table
        db_conn.execute(
            'INSERT INTO base_titles VALUES (?,?,?,?,?)',
            (
                title['name'],
                title['reference']['book'],
                title['reference']['page'],
                title['xp_to_completion'],
                title['title_ability'],
            )
        )

        # Write to title awards table
        db_conn.executemany(
            'INSERT INTO base_title_awards VALUES (?,?,?,?,?,?,?)',
            [
                (
                    title['name'],
                    award['award_attribute'],
                    award['base_award'],
                    award['constraint']['type'] if 'constraint' in award else None,
                    award['constraint']['value'] if 'constraint' in award and 'value' in award['constraint'] else None,
                    award['constraint']['range'][0] if 'constraint' in award and 'range' in award['constraint'] else None,
                    award['constraint']['range'][1] if 'constraint' in award and 'range' in award['constraint'] else None
                )
                for award in title['social_awards']
            ]
        )

        # Write to title advancement table
        db_conn.executemany(
            'INSERT INTO base_title_advancements VALUES (?,?,?,?,?)',
            [
                (
                    title['name'],
                    advancement['rank'] if 'rank' in advancement else None,
                    advancement['name'],
                    advancement['type'],
                    advancement['special_access']
                )
                for advancement in title['advancements']
            ]
        )


def patterns_to_db(db_conn):
    
    # Create item patterns table
    create_tables(
        db_conn,
        'item_patterns',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            xp_cost INTEGER,
            rarity_modifier INTEGER
        )''',
        desc_fields = 'name',
        tr_fields = ['name']
    )

    # Read item patterns from JSON
    with open('json/item_patterns.json', encoding = 'utf8') as f:
        item_patterns = json.load(f)
    
    # Write item patterns to item pattern and techniques table
    for pattern in item_patterns:

        # Write to item pattern table
        db_conn.execute(
            'INSERT INTO base_item_patterns VALUES (?,?,?,?,?)',
            (
                pattern['name'],
                pattern['reference']['book'],
                pattern['reference']['page'],
                pattern['xp_cost'],
                pattern['rarity_modifier']
            )
        )
        
        # Write to techniques table
        db_conn.execute(
            '''INSERT INTO base_techniques VALUES (
                :category,
                :subcategory,
                :name,
                :restriction,
                :reference_book,
                :reference_page,
                :rank,
                :xp
            )''',
            {
                'category': 'Item Patterns',
                'subcategory': None,
                'name': pattern['name'],
                'restriction': None,
                'reference_book': pattern['reference']['book'],
                'reference_page': pattern['reference']['page'],
                'rank': 1,
                'xp': pattern['xp_cost']
            }
        )



def bonds_to_db(db_conn):

    # Create bonds table
    create_tables(
        db_conn,
        'bonds',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            bond_ability_name TEXT,
            reference_book TEXT,
            reference_page INTEGER
        )''',
        desc_fields = {
            'name': '',
            'bond_ability_name': 'bond_ability'
        },
        tr_fields = ['name', 'bond_ability_name']
    )

    # Read bonds from JSON
    with open('json/bonds.json', encoding = 'utf8') as f:
        bonds = json.load(f)
    
    # Write to bonds table
    for bond in bonds:
        db_conn.execute(
            'INSERT INTO base_bonds VALUES (?,?,?,?)',
            (
                bond['name'],
                bond['ability'],
                bond['reference']['book'],
                bond['reference']['page']
            )
        )


def regions_to_db(db_conn):

    # Create regions table
    create_tables(
        db_conn,
        'regions',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            ring_increase TEXT,
            skill_increase TEXT,
            glory INTEGER,
            type TEXT,
            subtype TEXT,
            reference_book TEXT,
            reference_page INTEGER
        )''',
        desc_fields = 'name',
        tr_fields = ['name', 'ring_increase', 'skill_increase', 'type', 'subtype']
    )

    # Read regions from JSON
    with open('json/regions.json', encoding = 'utf8') as f:
        regions = json.load(f)
    
    # Write to regions table
    for region in regions:
        db_conn.execute(
            'INSERT INTO base_regions VALUES (?, ?, ?, ?, ?, ?, ?, ?)',
            (
                region['name'],
                region['ring_increase'],
                region['skill_increase'],
                region['glory'],
                region['type'],
                region['subtype'] if 'subtype' in region else None,
                region['reference']['book'],
                region['reference']['page']
            )
        )


def upbringings_to_db(db_conn):

    # Create upbringings table
    create_tables(
        db_conn,
        'upbringings',
        '''CREATE TABLE {} (
            name TEXT PRIMARY KEY,
            reference_book TEXT,
            reference_page INTEGER,
            status_modification INTEGER,
            starting_wealth_value INTEGER,
            starting_wealth_unit TEXT,
            starting_item TEXT
        )''',
        desc_fields = 'name',
        tr_fields = ['name']
    )
    create_tables(
        db_conn,
        'upbringing_skill_increases',
        '''CREATE TABLE {} (
            upbringing TEXT,
            set_id INTEGER,
            set_size INTEGER,
            skill TEXT
        )''',
        tr_fields = ['upbringing', 'skill']
    )
    create_tables(
        db_conn,
        'upbringing_rings',
        '''CREATE TABLE {} (
            upbringing TEXT,
            ring TEXT
        )''',
        tr_fields = ['upbringing', 'ring']
    )

    # Read upbringings JSON
    with open('json/upbringings.json', encoding = 'utf8') as f:
        upbringings = json.load(f)
    
    # Write to upbringings tables
    for upbringing in upbringings:

        # Write to upbringing table
        db_conn.execute(
            '''INSERT INTO base_upbringings VALUES (
                :name,
                :reference_book,
                :reference_page,
                :status_modification,
                :starting_wealth_value,
                :starting_wealth_unit,
                :starting_item
            )''',
            {
                'name': upbringing['name'],
                'reference_book': upbringing['reference']['book'],
                'reference_page': upbringing['reference']['page'],
                'status_modification': upbringing['status_modification'],
                'starting_wealth_value': upbringing['starting_wealth']['value'],
                'starting_wealth_unit': upbringing['starting_wealth']['unit'],
                'starting_item': upbringing['starting_item'] if 'starting_item' in upbringing else None
            }
        )

        # Write to upbringing skill increases table
        for skill_set_id, skill_set in enumerate(upbringing['skill_increases']):
            db_conn.executemany(
                '''INSERT INTO base_upbringing_skill_increases VALUES (
                    :upbringing,
                    :set_id,
                    :set_size,
                    :skill
                )''',
                [
                    {
                        'upbringing': upbringing['name'],
                        'set_id': skill_set_id,
                        'set_size': skill_set['size'],
                        'skill': skill
                    }
                    for skill in skill_set['set']
                ]
            )
        
        # Write to upbringing rings table
        db_conn.executemany(
            'INSERT INTO base_upbringing_rings VALUES (:upbringing, :ring)',
            [
                {
                    'upbringing': upbringing['name'],
                    'ring': ring
                }
                for ring in upbringing['ring_increase']['set']
            ]
        )


def desc_to_db(db_conn):
    db_conn.execute(
        '''CREATE TABLE user_descriptions (
            name TEXT PRIMARY KEY,
            description TEXT,
            short_desc TEXT
        )'''
    )


def translations_to_db(db_conn):
    db_conn.execute(
        '''CREATE TABLE i18n (
            string TEXT PRIMARY KEY,
            string_tr TEXT
        )'''
    )


def main():

    # Change working directory to data folder
    os.chdir(
        os.path.dirname(
            os.path.dirname(
                os.path.realpath(__file__)
            ))
    )

    # Open connection
    db_conn = connect_db('paperblossoms.db')

    # Write data

    print('Writing descriptions and translations')
    desc_to_db(db_conn)
    translations_to_db(db_conn)

    print('Writing rings')
    rings_to_db(db_conn)

    print('Writing skills')
    skills_to_db(db_conn)

    print('Writing techniques')
    techniques_to_db(db_conn)

    print('Writing advantages and disadvantages')
    advantages_to_db(db_conn)

    print('Writing question 8')
    q8_to_db(db_conn)

    print('Writing titles')
    titles_to_db(db_conn)

    print('Writing item patterns')
    patterns_to_db(db_conn)

    print('Writing bonds')
    bonds_to_db(db_conn)

    print('Writing regions')
    regions_to_db(db_conn)

    print('Writing item qualities')
    qualities_to_db(db_conn)

    print('Writing personal effects')
    personal_effects_to_db(db_conn)

    print('Writing armor')
    armor_to_db(db_conn)

    print('Writing weapons')
    weapons_to_db(db_conn)

    print('Writing clans')
    clans_to_db(db_conn)

    print('Writing heritage tables')
    heritage_to_db(db_conn)

    print('Writing schools')
    schools_to_db(db_conn)

    print('Writing upbringings')
    upbringings_to_db(db_conn)

    # Commit and close connection
    print('Saving and closing database connection')
    db_conn.commit()
    db_conn.close()


if __name__ == '__main__':
    main()
