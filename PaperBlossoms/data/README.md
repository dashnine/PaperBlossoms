# Data

Last updated: CR release (v1.31)  
Author: @meow9th

The application database is [paperblossoms.db](paperblossoms.db), a SQLite database. However, in order to make data entry easier, more reliable and more maintainable, data is never entered directly into `paperblossoms.db`. Instead, data is entered in [JSON](json/) files, which are then flattened and normalized into `paperblossoms.db` with the Python script [json_to_db.py](scripts/json_to_db.py). Below, I describe the structure of the application database and the data entry system I devised to minimize tedium and maximize reliability.

The documentation starts with guidelines for [§Contributing data](#contributing-data), but the system is elaborate (because coding is more fun than data entry), so you'll probably want to reference the [§Application database](#application-database) and the [§Data entry system](#data-entry-system) as needed. Throughout this guide, I assume working knowledge of JSON, JSON schemas, SQL databases, Python and VS Code.

Directory contents:

- [i18n/](i18n/): See [§Translations](#translations)
- [json/](json/): See [§JSON data](#json-data)
- [json_schema/](json_schema/): See [§JSON schemas](#json-schemas)
- [scripts/](scripts/): See [§Scripts](#scripts)
- [paperblossoms.db](paperblossoms.db): See [§Application database](#application-database)
- [vscode_workspace_settings.json](vscode_workspace_settings.json): See [§JSON schemas](#json-schemas)

## Contributing data

Below, I describe how to contribute base data. Custom data is user-specific, and is not meant to be released with the application. [§Translations](#translations) have a [separate contribution process](https://github.com/dashnine/PaperBlossoms/wiki/Translation), and can be provided for both the UI and the database. See [§Application database](#application-database) for more detail about the sources of data and how they're combined. Thus, the contribution guide here focuses on base data.

Contribution guidelines:

- Never modify `paperblossoms.db` directly. We will only merge PRs after validating JSON data and regenerating `paperblossoms.db`.
- Coordinate with @dashnine about changes to the application database schema or the JSON schemas. Open [an issue](https://github.com/dashnine/PaperBlossoms/issues/new/choose) or [draft PR](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/about-pull-requests#draft-pull-requests) to make your intention to do so explicit.
- It is polite to refresh `enum`s in JSON schemas if you add any data which go into `enum`s. Doing so will ease data entry for dependent data (e.g., refresh techniques if you add new techniques, which will make adding new schools which reference those new techniques possible). See [§JSON schemas](#json-schemas) for more detail.
- We will reject commits in which user descriptions (`*description` or `*short_desc`) are included. We refrain from including that data in order not to infringe upon FFG's copyright.

### Contribute a new instance of an existing entity

Follow these steps if you'd like to add a new school, clan, weapon, or any new instance of an entity already captured in an existing JSON file. See [§JSON data](#json-data) for more details.

0. Set up the mapping between JSON files and their schemas in VS Code (see [§JSON schemas](#json-schemas)).
> Why VS Code?
>
> I recommend using VS Code because I take advantage of its autocompletion features, customized through `defaultSnippets` in the JSON schemas. See [§JSON schemas](#json-schemas) for more details. You can use any editor you want, as long as I approve the JSON schemas and your JSON contributions pass validation.
1. Add the new object(s) to the relevant JSON file.
    - Trigger an autocompletion proposal with <kbd>ctrl</kbd>-<kbd>space</kbd>.
    - Type characters to narrow down string autocompletion proposals.
    - Navigate between fields in the snippet with <kbd>tab</kbd>.
    - Nested JSON objects come with snippets too. Trigger the snippet for a nested object with <kbd>ctrl</kbd>-<kbd>space</kbd> once your cursor is within the `{}` that indicate the nested object.
2. Validate the JSON data with `validate_json.py`.
3. Generate the application database with `json_to_db.py`.

Optional:

4. If you're adding a new instance of an entity in the list of `enum`s, refresh the `enum`s in the affected JSON schemas with `add_enums.py` using the appropriate option.

See [§Scripts](#scripts) for more detail and example usage for each of the scripts mentioned above.

### Contribute a new entity

> Warning: New entities constitute changes to the application database schema. Coordinate with @dashnine via issue or PR before attempting this contribution.

1. Write a new [§JSON schema](#json-schemas).
    - Run `add_default_snippets_to_schema.py`
    - Modify `add_enums.py` to write `enum`s to the new JSON schema as applicable. Run the updated `add_enums.py`.
2. Set up a mapping beween the new JSON schema and the new JSON file in both your own workspace settings and also [vscode_workspace_settings.json](vscode_workspace_settings.json).
3. Enter data in the new JSON file.
    - Trigger an autocompletion proposal with <kbd>ctrl</kbd>-<kbd>space</kbd>.
    - Type characters to narrow dConsult wiown string autocompletion proposals.
    - Navigate between fields in the snippet with <kbd>tab</kbd>.
    - Nested JSON objects come with snippets too. Trigger the snippet for a nested object with <kbd>ctrl</kbd>-<kbd>space</kbd> once your cursor is within the `{}` that indicate the object.
4. Run `validate_json.py`.
5. Update `json_to_db.py` to flatten and normalize the new JSON file to the application database. Run the updated `json_to_db.py`.
    - Add a `*_to_db()` function which creates the corresponding tables and writes the JSON data to those tables.
    - Add a call to the new `*_to_db()` function in `main()`.

Optional:

6. Update `add_enums.py` to read `enum`s from the new JSON file as applicable. Run the updated `add_enums.py` to add those options to other JSON schemas as applicable.

### Contribute a new book

Published books are perhaps the most frequently employed `enum` across all JSON schemas. The list of valid books is hard-coded in `add_enums.py`, in the function `get_books()`.

1. Add the book title abbreviation to the list `books_enum` in `get_books()` in `add_enums.py`.
2. Run `add_enums.py --option books` to add the new book to all JSON schemas.

To add data from the new book, see sections above on [adding new instances](#contribute-a-new-instance-of-an-existing-entity) and [adding new entities](#contribute-a-new-entity).

### Common hitches

Below I list a miscellaneous grab-bag of common headaches and 'what now?' phenomena that I run into pretty much any time I enter data for a new book:

- School starting outfits are the worst (make that: schools are the worst, but starting outfits are the worst part of schools). Starting outfits frequently include equipment which is not included or described anywhere else. In those cases, I default to creating a new item in `personal_effects.json` with the name and the reference pointing to the school page. The exception is general descriptions like `Two Items of Rarity 4 or Lower`. For those, I coordinate with @dashnine, who handles them specially in the application code, and add them as a legal option in `add_enums.py`.
- Multi-cursor in VS Code frequently triggers for me when editing JSON files with snippets, at which point exiting out of multi-cursor mode also exits me out of tab navigation within a snippet. If you figure out how to prevent this annoying behaviour, let me know.
- All files are encoded in UTF-8. I typically enter non-ASCII characters directly from character map utilities.
- Speaking of encoding in UTF-8, the Python function `json.load()` defaults to `encoding = 'utf8'` on *nix systems but not on Windows. I have not yet refactored `json.load()` to specify `encoding = 'utf8'` everywhere (only the ones I happened to work on while on Windows), so if you are on Windows and run any scripts which call `json.load()` without the `encoding` argument, you'll get an error. Simply fix the offending function call to specify encoding and try again. (Then include it in your PR!)
- None of the data entry system has been tested on Mac, but in theory all the technology is cross-platform. See encoding note above for why I'm skeptical that the system will work trouble-free.

## Application database

`paperblossoms.db` is the application database. Throughout this guide, I use the terms 'application database' and `paperblossoms.db` interchangeably.

There are three sources of data which you will find in the application database:

- Base data (`base_*` tables): The data entered from the books, included as part of the application release. This is the data that is entered in JSON and then imported into `paperblossoms.db`.
- Custom user data (`user_*` tables, `*desc*` columns): Data that the user enters in their own local copy of Paper Blossoms. Custom additions to the game, like new schools or new advantages, are stored in `user_` tables. `*desc*` columns exist for storing descriptions (e.g., for each technique). The Paper Blossoms team does not distribute this data to avoid infringing on FFG's copyright on the L5R books, but users who own the books may choose to enter verbatim text if they so wish, and store it with the application for display on, e.g., their character sheets. 
- Translations (`i18n` table, `*_tr` columns): Paper Blossoms is lucky enough to benefit from generous volunteer translators. The `i18n` table is populated with a table of translations from the [i18n](i18n/) directory at application launch. For more detail on how translations are populated and fetched, see [§Translations](#translations) below.

The data entry system is focused on entering base data, but support for custom user data and translations are programmatically generated by `json_to_db.py`: `base_*` and `user_*` tables have identical fields, and are joined together in views without a prefix. The application references these views, so base data and user data are combined and treated equivalently. The views also include joins to the `i18n` table, so the application can fetch translated strings if they exist.

`paperblossoms.db` is a standard SQLite file, and can be inspected with any tool compatible with SQLite. The Paper Blossoms team uses [DB Browser for SQLite](https://sqlitebrowser.org) and the [SQLite extension for VS Code](https://marketplace.visualstudio.com/items?itemName=alexcvzz.vscode-sqlite), but you can use any tool that works with SQLite.

## Data entry system

### JSON data

Each JSON file represents an array of objects, representing high-level entities in the game, such as clans, schools and weapons. Here's one of the simplest examples, [rings.json](json/rings.json):

```
[
    {
        "name": "Air",
        "outstanding_quality": "Grace, eloquence or empathy"
    },
    {
        "name": "Earth",
        "outstanding_quality": "Thoroughness, patience or calm"
    },
    {
        "name": "Fire",
        "outstanding_quality": "Creativity, passion or drive"
    },
    {
        "name": "Water",
        "outstanding_quality": "Adaptability, friendliness or awareness"
    },
    {
        "name": "Void",
        "outstanding_quality": "Self-awareness, insight or mysticism"
    }
]
```

For one of the most complicated examples, check out [schools.json](json/schools.json).

### JSON schemas

Every [JSON](json/) file pairs with a [JSON schema](json_schema/) of the same name. The JSON schemas serve multiple purposes:

- They're leveraged to programmatically generate ['default snippets'](https://code.visualstudio.com/docs/languages/json#_define-snippets-in-json-schemas) for VS Code (see [add_default_snippets_to_schema.py](scripts/add_default_snippets_to_schema.py)). These snippets generate autocompletion proposals which include keys and values, and substantially reduce typing (and therefore errors) during data entry.
- They're used to generate autocompletion proposals for and validate strings with the [`enum` keyword](https://json-schema.org/understanding-json-schema/reference/generic.html#id4), substantially increasing the consistency of how string data is capitalized, punctuated and spelled.
- They're used to validate the entire JSON file (see [validate_json.py](scripts/validate_json.py)).
- They document the intended meaning of each field.

Here's a relatively simple example which features default snippets and `enum`s, [bonds.schema.json](json_schema/bonds.schema.json):

```
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Bonds",
    "description": "List of bond types",
    "type": "array",
    "items": {
        "description": "Type of bond",
        "type": "object",
        "properties": {
            "name": {
                "description": "Name of bond type",
                "type": "string"
            },
            "ability": {
                "description": "Bond ability",
                "type": "string"
            },
            "reference": {
                "description": "Page reference for bond",
                "type": "object",
                "properties": {
                    "book": {
                        "description": "Book in which reference can be found",
                        "type": "string",
                        "enum": [
                            "Core",
                            "EE",
                            "SL",
                            "Mantis",
                            "GMK",
                            "CoS",
                            "PoW"
                        ]
                    },
                    "page": {
                        "description": "Page on which reference can be found",
                        "type": "integer"
                    }
                },
                "required": [
                    "book",
                    "page"
                ],
                "defaultSnippets": [
                    {
                        "label": "New Page reference for bond",
                        "body": {
                            "book": "$1",
                            "page": "^$2"
                        }
                    }
                ]
            }
        },
        "required": [
            "name",
            "ability",
            "reference"
        ],
        "defaultSnippets": [
            {
                "label": "New Type of bond",
                "body": {
                    "name": "$1",
                    "ability": "$2",
                    "reference": "^$3"
                }
            }
        ]
    }
}
```

Again, the most complicated example is [schools.schema.json](json_schema/schools.schema.json).

In order to get the magical default snippet behaviour, VS Code must be [configured to map](https://code.visualstudio.com/docs/languages/json#_mapping-to-a-schema-in-the-workspace) a JSON file with its schema. Personally, I map using the `json.schemas` field in [workspace settings](https://code.visualstudio.com/docs/getstarted/settings). To improve portability, I provide `vscode_workspace_settings.json` which contain all the mappings, as well as a few settings I find useful. Your mileage may vary. Copy what you like from the provided file into your own workspace settings file.

### Scripts

A handful of [helper scripts](scripts/) automate common tasks. These tasks fall into two categories: tasks which are part of the data entry workflow and tasks which update the data entry system. 

The general data entry workflow is as follows:

1. Modify JSON file(s): manual
2. Validate JSON file(s): [validate_json.py](scripts/validate_json.py)
3. Export data from JSON files to `paperblossoms.db`: [json_to_db.py](scripts/json_to_db.py)

`validate_json.py` loops through schemas found in the `json_schema/` directory and validates the JSON of the same name in the `json/` directory. It will raise an informative error if a JSON fails validation. This script takes no arguments, and outputs the schema name as it loops.

```
$ python3 PaperBlossoms/data/scripts/validate_json.py 
Validated qualities.json
Validated armor.json
Validated techniques.json
Validated bonds.json
Validated personal_effects.json
Validated titles.json
Validated samurai_heritage.json
Validated schools.json
Validated item_patterns.json
Validated upbringings.json
Validated rings.json
Validated skill_groups.json
Validated regions.json
Validated clans.json
Validated weapons.json
Validated advantages_disadvantages.json
```

`json_to_db.py` is the workhorse script for converting JSON data into SQLite data. It constructs the entire SQLite database, including base data, custom data, and translations. It loads data from each JSON and writes that data to SQLite tables in a flattened and normalized way. It takes no arguments and outputs entities being written to the database as it goes through the JSON files.

```
$ python3 PaperBlossoms/data/scripts/json_to_db.py 
Opening database connection
Writing descriptions and translations
Writing rings
Writing skills
Writing techniques
Writing advantages and disadvantages
Writing question 8
Writing titles
Writing item patterns
Writing bonds
Writing regions
Writing item qualities
Writing personal effects
Writing armor
Writing weapons
Writing clans
Writing heritage tables
Writing schools
Writing upbringings
Saving and closing database connection
```

Other scripts modify the data entry system via the JSON schemas.

`add_default_snippets_to_schema.py` adds or overwrites default snippets to a specified JSON schema based on the schema. This script is intended to be used after a new schema is written or an existing schema is updated. Rather than manually adding or updating the default snippets, use this script to automatically generate those snippets. They can be modified afterwards in order to set default values, remove unwanted fields, etc. This script takes one argument, the JSON schema file, and will add default snippets for all objects in the JSON schema, no matter the level of nesting.

```
$ python3 PaperBlossoms/data/scripts/add_default_snippets_to_schema.py PaperBlossoms/data/json_schema/bonds.schema.json
```

`add_enums.py` adds or overwrites `enum` lists in JSON schemas based on entered data (primarily JSON). For example, to generate a valid list of skills and techniques for school curriculum advancements, this script will load `skills.json` and `techniques.json`, extract the names of all skills and techniques, and write those names as the `enum` list for `advance` in `curriculum` within `schools.json`. This script is intended to be used when an `enum` list needs to be refreshed (new techniques, new books, etc). An example usage and possible arguments listed below:

```
$ python3 PaperBlossoms/data/scripts/add_enums.py --option books
Writing books ...
... to clans
... to advantages/disadvantages
... to armor
... to personal effects
... to weapons
... to techniques
... to schools
... to titles
... to bonds
... to item patterns
$ python3 PaperBlossoms/data/scripts/add_enums.py -h
usage: add_enums.py [-h] [--option [{rings,clans,skills,techniques,qualities,equipment,advantages,books,resistance,currency} [{rings,clans,skills,techniques,qualities,equipment,advantages,books,resistance,currency} ...]]]

Utility to add enums to json schema

optional arguments:
  -h, --help            show this help message and exit
  --option [{rings,clans,skills,techniques,qualities,equipment,advantages,books,resistance,currency} [{rings,clans,skills,techniques,qualities,equipment,advantages,books,resistance,currency} ...]]
                        Which enums you want to write to json schemas (defaults to all with no arguments specified)
```

### Translations

Translations for the UI and the database are stored separately. In this section, I describe how translations are handled within the database.

How translations are stored/added:

- Database translations are stored as CSV files in the [i18n](i18n/) directory.
- Translations are simply `string` to `string_tr` mappings.
- All possible strings which could be translated are designated with `*_tr` fields in the database. Paper Blossoms includes a utility to generate a spreadsheet with a list strings to be translated.
- Translators fill out the spreadsheet with translations and check it in as a new CSV.

How the application fetches translations:

- At application launch, Paper Blossoms automatically loads the appropriate CSV into the `i18n` table in the application database (based on the same logic ).
- The `i18n` table is part of the view definition for each entity. Every field in the application database that could be translated has a translation field with the `*_tr` suffix, which is populated in the view definition of the entity with the appropriate `i18n.string_tr` value, or the untranslated value if a translation isn't available.
- During runtime, the application queries the application database, fetching the `*_tr` values.