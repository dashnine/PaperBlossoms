import pathlib
import json
import jsonschema



# Validates specified json against specified schema. Will raise informative error
# if json fails schema validation.
def validate_schema(json_filepath, schema_filepath):
    with open(json_filepath, encoding = 'utf8') as f:
        instance = json.load(f)
    with open(schema_filepath, encoding = 'utf8') as f:
        schema = json.load(f)
    
    try:
        jsonschema.validate(instance, schema)
        print('Validated ' + json_filepath.name)
    except jsonschema.exceptions.ValidationError as err:
        print('Could not validate ' + json_filepath.name + '!')
        print(err.message)

    return None


def main():

    # Get path to data directory
    data_dir = pathlib.Path(__file__).parents[1]

    # Loop through all json schemas
    for schema_filepath in list(data_dir.joinpath('json_schema').glob('*')):
        validate_schema(
            json_filepath=(
                data_dir
                .joinpath('json')
                .joinpath(schema_filepath.stem.split('.')[0] + '.json')
            ),
            schema_filepath = schema_filepath
        )

if __name__ == '__main__':
    main()
