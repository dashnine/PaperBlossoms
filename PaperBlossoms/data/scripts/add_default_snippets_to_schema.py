import sys
import json

# Helper to generate placeholder text based on property type
def get_placeholder(property_type, idx):
    
    if property_type == 'string':
        return('$' + str(idx))
    
    if property_type == 'array':
        return('^[${}]'.format(str(idx)))
    
    return('^$' + str(idx))


# Recursively add defaultSnippets property to objects in schema
def add_snippet(schema_object):
    
    if schema_object['type'] == 'array':
        schema_object['items'] = add_snippet(schema_object['items'])
        
    if schema_object['type'] in ('string', 'boolean', 'integer'):
        return(schema_object)
    
    if schema_object['type'] == 'object':
        property_types = {
            object_property: schema_object['properties'][object_property]['type']
            for object_property in schema_object['properties']
        }

        snippet_body = {
            object_property: get_placeholder(property_types[object_property], idx)
            for idx, object_property in enumerate(property_types, start = 1)
        }
    
        schema_object['defaultSnippets'] = [{
            'label': 'New ' + schema_object['description'],
            'body': snippet_body
        }]
    
        for object_property in property_types:
            schema_object['properties'][object_property] = add_snippet(schema_object['properties'][object_property])
        
    return(schema_object)


# Load, add defaultSnippets and overwrite schema file
def add_snippet_to_schema(schema_filename):
    with open(schema_filename, encoding = 'utf8') as f:
        schema = json.load(f)
    schema = add_snippet(schema)
    with open(schema_filename, 'w', encoding = 'utf8') as f:
        json.dump(schema, f, indent = 4)


def main():
    add_snippet_to_schema(sys.argv[1])


if __name__ == '__main__':
    main()