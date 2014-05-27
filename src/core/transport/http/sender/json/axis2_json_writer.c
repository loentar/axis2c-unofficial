/*
 *  Copyright 2013-2014 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <axutil_utils_defines.h>
#include <axiom_element.h>
#include <axiom_attribute.h>
#include <json.h>
#include <axis2_json_writer.h>

#define AXIS2_JSON_XSI_URI "http://www.w3.org/2001/XMLSchema-instance"
#define AXIS2_JSON_SOAPENC_URI "http://schemas.xmlsoap.org/soap/encoding/"

struct axis2_json_writer
{
    json_object* json_obj;
};

json_type axis2_json_string_to_type(const axis2_char_t* type_str)
{
    if (!type_str)
        return json_type_string;
    if (!strcmp(type_str, "null"))
        return json_type_null;
    if (!strcmp(type_str, "boolean"))
        return json_type_boolean;
    if (!strcmp(type_str, "double"))
        return json_type_double;
    if (!strcmp(type_str, "int"))
        return json_type_int;
    if (!strcmp(type_str, "object"))
        return json_type_object;
    if (!strcmp(type_str, "array"))
        return json_type_array;
    return json_type_string;
}

axis2_char_t* axis2_json_element_get_type(axiom_element_t* om_element, const axutil_env_t* env)
{
    axiom_attribute_t* attr = NULL;
    axutil_hash_index_t* index;
    axutil_hash_t* attr_hash = axiom_element_get_all_attributes(om_element, env);
    axiom_namespace_t* ns;
    axis2_char_t* local_name;

    if (!attr_hash)
        return AXIS2_FALSE;

    for (index = axutil_hash_first(attr_hash, env);
         index; index = axutil_hash_next(env, index))
    {
        axutil_hash_this(index, NULL, NULL, (void**)&attr);
        if (!attr)
            continue;

        ns = axiom_attribute_get_namespace(attr, env);

        if (ns && !strcmp(axiom_namespace_get_uri(ns, env), AXIS2_JSON_XSI_URI))
        {
            local_name = axiom_attribute_get_localname(attr, env);
            if (!strcmp(local_name, "type"))
                return axiom_attribute_get_value(attr, env);
            if (!strcmp(local_name, "nil"))
            {
                axis2_char_t* attr_value =
                        axiom_attribute_get_value(attr, env);
                if (!strcmp(attr_value, "true") || !strcmp(attr_value, "1"))
                    return "null";
            }
        }
    }

    return NULL;
}

axis2_bool_t axis2_json_element_is_array(axiom_element_t* om_element, const axutil_env_t* env)
{
    axiom_attribute_t* attr = NULL;
    axutil_hash_index_t* index;
    axutil_hash_t* attr_hash = axiom_element_get_all_attributes(om_element, env);

    if (!attr_hash)
        return AXIS2_FALSE;

    for (index = axutil_hash_first(attr_hash, env);
         index; index = axutil_hash_next(env, index))
    {
        axutil_hash_this(index, NULL, NULL, (void**)&attr);
        if (attr && !strcmp(axiom_attribute_get_localname(attr, env), "arrayType"))
        {
            /* found some "arrayType" attribute, check it's namespace */
            axutil_qname_t* qname = axiom_attribute_get_qname(attr, env);
            if (qname && !strcmp(axutil_qname_get_uri(qname, env), AXIS2_JSON_SOAPENC_URI))
                return AXIS2_TRUE;
        }
    }

    return AXIS2_FALSE;
}

void axis2_json_write_node(json_object* parent, axiom_node_t* om_node, const axutil_env_t* env,
                           axis2_bool_t parent_is_array)
{
    axiom_element_t* elem;
    axiom_node_t* child_node;
    const axis2_char_t* local_name;
    json_object* obj;
    json_object* array = NULL;
    axis2_bool_t is_array = AXIS2_FALSE;

    if (!om_node || axiom_node_get_node_type(om_node, env) != AXIOM_ELEMENT)
        return;

    elem = (axiom_element_t*)axiom_node_get_data_element(om_node, env);
    local_name = axiom_element_get_localname(elem, env);

    child_node = axiom_node_get_first_element(om_node, env);

    if (!parent_is_array)
    {
        /* find existing object */
        if (json_object_object_get_ex(parent, local_name, &obj))
        {
            /* check that object is array? */
            if (!json_object_is_type(obj, json_type_array))
            {
                /* convert to array */
                obj = json_object_get(obj);
                array = json_object_new_array();
                json_object_array_add(array, obj);
                json_object_object_del(parent, local_name);
                json_object_object_add(parent, local_name, array);
            }
            else
                array = obj;
        }
        else
        {
            if (axis2_json_element_is_array(elem, env))
            {
                array = json_object_new_array();
                json_object_object_add(parent, local_name, array);
                is_array = AXIS2_TRUE;
            }
        }
    }

    if (!child_node)
    {
        /* this is a leaf node */
        json_object* json_value = NULL;
        json_type type;

        if (is_array) /* don't add array element as empty item */
            return;

        type = axis2_json_string_to_type(axis2_json_element_get_type(elem, env));

        /* check for nillable */
        if (type != json_type_null)
        {
            const axis2_char_t* value =
                    axiom_element_get_text(elem, env, om_node);
            if (!value)
                value = "";

            switch (type) {
            case json_type_boolean:
                json_value = json_object_new_boolean(!strcmp(value, "true")
                                                     || !strcmp(value, "1"));
                break;

            case json_type_double:
            {
                double double_value = 0.;
                json_parse_double(value, &double_value);
                json_value = json_object_new_double(double_value);
                break;
            }

            case json_type_int:
            {
                int64_t int_value = 0;
                json_parse_int64(value, &int_value);
                json_value = json_object_new_int64(int_value);
                break;
            }

            default:
                json_value = json_object_new_string(value);
            }
        }


        if (parent_is_array)
            json_object_array_add(parent, json_value);
        else
        if (array)
            json_object_array_add(array, json_value);
        else
            json_object_object_add(parent, local_name, json_value);

        return;
    }

    /* iterate through children elements */
    if (!is_array)
    {
        obj = json_object_new_object();
        if (parent_is_array)
            json_object_array_add(parent, obj);
        else
        if (array)
            json_object_array_add(array, obj);
        else
            json_object_object_add(parent, local_name, obj);
    }
    else
    {
        obj = array;
    }

    for (; child_node; child_node = axiom_node_get_next_sibling(child_node, env))
        if (axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
            axis2_json_write_node(obj, child_node, env, is_array);
}


AXIS2_EXTERN axis2_json_writer_t* AXIS2_CALL
axis2_json_writer_create(
        const axutil_env_t* env)
{
    axis2_json_writer_t* writer = (axis2_json_writer_t*)AXIS2_MALLOC(env->allocator,
                                                                     sizeof(struct axis2_json_writer));
    if (writer)
        writer->json_obj = NULL;

    return writer;
}


AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_free(
        axis2_json_writer_t* writer,
        const axutil_env_t* env)
{
    json_object_put(writer->json_obj);
    AXIS2_FREE(env->allocator, writer);
}


AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_write(
        axis2_json_writer_t* writer,
        const axiom_node_t* node,
        const axutil_env_t* env)
{
    /* free existing object */
    if (writer->json_obj)
        json_object_put(writer->json_obj);

    writer->json_obj = json_object_new_object();
    axis2_json_write_node(writer->json_obj, (axiom_node_t*)node, env, AXIS2_FALSE);
}


AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
axis2_json_writer_get_json_string(
        axis2_json_writer_t* writer,
        const axutil_env_t* env,
        int* json_string_length)
{
    const axis2_char_t* result =
            (const axis2_char_t*)json_object_to_json_string_ext(writer->json_obj,
                                                               JSON_C_TO_STRING_PLAIN);
    (void)env;

    if (json_string_length)
        *json_string_length = strlen(result);

    return result;
}
