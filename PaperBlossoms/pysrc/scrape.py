from pypdf import PdfReader
from typing import Any
import glob
import json
import os, sys
import re
import cryptography


SCHOOL_IGNORED_PROPERTIES = ["starting techniques", "starting skills", "rings", "kata", "shūji", 
                             "ninjutsu", "ritual", "invocation", "kihō"]
                             
substitutions = {
     "Hands of Tides": {"name": "Hands of the Tides"},
     "Disdain for Compassion": {"name": "Disdain for a Bushidō Tenet"},
     "Disdain for Courage": {"name": "Disdain for a Bushidō Tenet"},
     "Disdain for Courtesy": {"name": "Disdain for a Bushidō Tenet"},
     "Disdain for Duty and Loyalty": {"name": "Disdain for a Bushidō Tenet"},
     "Disdain for Honor": {"name": "Disdain for a Bushidō Tenet"},
     "Disdain for Righteousness": {"name": "Disdain for a Bushidō Tenet"},
     "Disdain for Sincerity": {"name": "Disdain for a Bushidō Tenet"},
     "Paragon of Compassion": {"name": "Paragon of a Bushidō Tenet"},
     "Paragon of Courage": {"name": "Paragon of a Bushidō Tenet"},
     "Paragon of Courtesy": {"name": "Paragon of a Bushidō Tenet"},
     "Paragon of Duty and Loyalty": {"name": "Paragon of a Bushidō Tenet"},
     "Paragon of Honor": {"name": "Paragon of a Bushidō Tenet"},
     "Paragon of Righteousness": {"name": "Paragon of a Bushidō Tenet"},
     "Paragon of Sincerity": {"name": "Paragon of a Bushidō Tenet"},
     "Ikomo Bard School": {"name": "Ikoma Bard School"},
     "Kitsu Real Wanderer School": {"name": "Kitsu Realm Wanderer School", "book": "CR"},
     "Elxir of Recovery": {"name": "Elixir of Recovery"},
     "Landslide Strike": {"page": 89},
     "Support of the Kakita Dueling Academy": {"name": "Support of [One Group]", "book": "Core", "page": 110}
    }


def get_clan_blurbs(availablePDFs: dict[str, dict[str, Any]]) -> dict:
    blurbs = []
    clanDicts = get_json("clans")
    for clanDict in clanDicts:
        (name, search_name, reader, book, page_num) = find_page(clanDict, availablePDFs)
        if reader is None:
            pass
            # print(f"Clan {clanDict["name"]} in unavailable source {clanDict["reference"]["book"]}.")
        else:
            blurbs.append(get_blurb(
                name, search_name, "clan", page_num, book, reader, beginning_properties = True))
        for familyDict in clanDict["families"]:
            (name, search_name, reader, book, page_num) = find_page(familyDict, availablePDFs)
            if reader is None:
                pass
                # print(f"Family {familyDict["name"]} of clan {clanDict["name"]} in unavailable source {clanDict["reference"]["book"]}.")
            else:
                blurbs.append(get_blurb(
                    name, search_name, "family", page_num, book,
                    reader, beginning_properties = True, 
                    ignore_properties_list = ["ring increase", "skill increase", "glory"]))
    return blurbs

def get_school_blurbs(availablePDFs: dict[str, dict[str, Any]]):
    blurbs = []
    schoolDicts = get_json("schools")
    for schoolDict in schoolDicts:
        (name, search_name, reader, book, page_num) = find_page(schoolDict, availablePDFs)
        if reader is None:
            pass
            # print(f"School {schoolDict["name"]} in unavailable source {schoolDict["reference"]["book"]}.")
        else:
            blurbs.append(get_blurb(
                name, search_name, "school", page_num, book, reader, 
                ignore_properties_list = SCHOOL_IGNORED_PROPERTIES))
    return blurbs

def get_adv_disadv_blurbs(availablePDFs: dict[str, dict[str, Any]]):
    blurbs = []
    advListDicts = get_json("advantages_disadvantages")
    for advListDict in advListDicts:
        itemType = advListDict["name"]
        for advDict in advListDict["entries"]:
            (name, search_name, reader, book, page_num) = find_page(advDict, availablePDFs)
            if reader is None:
                pass
                # print(f"{itemType} {advDict["name"]} in unavailable source {advDict["reference"]["book"]}.")
            else:
                blurbs.append(get_blurb(
                    name, search_name, itemType.lower()[:-1], page_num, book, reader, cut_to_list = True))
    return blurbs


def get_technique_blurbs(availablePDFs: dict[str, dict[str, Any]]):
    blurbs = []
    techTypeDicts = get_json("techniques")
    for techTypeDict in techTypeDicts:
        type_str = techTypeDict["name"]
        for subCatDict in techTypeDict["subcategories"]:
            for techDict in subCatDict["techniques"]:
                (name, search_name, reader, book, page_num) = find_page(techDict, availablePDFs)
                if reader is None:
                    pass
                    # print(f"{type_str} {techDict["name"]} in unavailable source {techDict["reference"]["book"]}.")
                else:
                    blurbs.append(get_blurb(name, search_name, type_str, page_num, book, reader))
    return blurbs


def get_item_blurbs(itemType: str, availablePDFs: dict[str, dict[str, Any]], **kwargs):
    blurbs = []
    itemDicts = get_json(f"{itemType}s")
    for itemDict in itemDicts:
        (name, search_name, reader, book, page_num) = find_page(itemDict, availablePDFs)
        if reader is None:
            print(f"{itemType.title()} {itemDict["name"]} in unavailable source {itemDict["reference"]["book"]}.")
        else:
            blurbs.append(get_blurb(name, search_name, itemType, page_num, book, reader, **kwargs))
    return blurbs
    
def new_user_description_file(pdfs: dict[str, dict[str, Any]]) -> int:
    blurbs = []
    blurbs += get_clan_blurbs(pdfs)
    blurbs += get_adv_disadv_blurbs(pdfs)
    blurbs += get_technique_blurbs(pdfs)
    blurbs += get_school_blurbs(pdfs)
    blurbs = [blurb for blurb in blurbs if blurb is not None]
    f = open("user_descriptions.csv", "w", encoding="utf-8")
    for blurb in blurbs:
        escaped_text = blurb["text"].replace('"', "'")
        escaped_text = escaped_text.replace('\n', '%0A')
        f.write(f'"{blurb["name"]}", "{escaped_text}%0A%0A", ""\n')
    f.close()
    return len(blurbs)

def make_user_description_file():
    pdfs = find_local_pdfs()
    new_user_description_file(pdfs)


def get_json(filename: str) -> list[dict[str, Any]]:
    sys.stderr.write(os.getcwd())
    path = f"./json/{filename}.json"
    if not os.path.isfile(path):
        path = f"./{filename}.json"
    if not os.path.isfile(path):
        raise Exception(f"Could not find {filename}.json")
    f = open(path, "r", encoding="utf-8")
    dictList = json.loads(f.read())
    f.close()
    return dictList
    
                
def find_page(jsonEntry: dict[str, Any], pdfs: dict[str, PdfReader]) -> tuple[PdfReader, int]:
    name = jsonEntry["name"]
    search_name = name
    book = jsonEntry["reference"]["book"]
    page = jsonEntry["reference"]["page"]
    substDict = substitutions.get(name)
    if substDict is not None:
        search_name = substDict.get("name", name)
        book = substDict.get("book", book)
        page = substDict.get("page", page)
    readerDict = pdfs.get(book, None)
    if readerDict is not None:
        return (name, search_name, readerDict["reader"], book, page + readerDict["page_offset"])
    else:
        return (None, None, None, None, 0)

def get_pdf_info(path) -> dict[str, Any]:
    reader = PdfReader(path)
    file_id = get_id(reader)
    if file_id is None:
        return None
    page_offset = determine_page_offset(reader)
    return {"reader": reader, "page_offset": page_offset, "file_id": file_id}

def find_dir_pdfs(dir_path) -> dict[str, PdfReader]:
    availablePDFs = {}
    for file in glob.glob(dir_path+'/*.pdf'):
        info = get_pdf_info(file)
        availablePDFs[info["file_id"]] = info
    return availablePDFs
    
def find_local_pdfs() -> dict[str, PdfReader]:
    return find_dir_pdfs(".") + find_dir_pdfs("./pdfs")

def handle_file(path: str) -> tuple[str, int, str]:
    # from pypdf._crypt_providers._cryptography import (
        # CryptAES,
        # CryptRC4,
        # aes_cbc_decrypt,
        # aes_cbc_encrypt,
        # aes_ecb_decrypt,
        # aes_ecb_encrypt,
        # crypt_provider,
        # rc4_decrypt,
        # rc4_encrypt,
    # )
    # from pypdf._utils import Version
    pdfs = {}
    info = get_pdf_info(path)
    if info is None:
        return ("", 0, "")
    pdfs[info["file_id"]] = info
    count = new_user_description_file(pdfs)
    return (info["file_id"], count, "user_descriptions.csv")
    
def determine_page_offset(reader: PdfReader) -> int:
    cur_page_num = -1
    page_offset_confirmations = dict()
    for i in range(-5, 5):
        page_offset_confirmations[i] = 0
    while True:
        cur_page_num += 1
        text_items = find_text_items(reader, [cur_page_num], text_only = True)
        for possible_page_offset in range(-5, 5):
            check_num = cur_page_num - possible_page_offset
            if check_num < 0:
                continue
            if str(check_num) in text_items:
                page_offset_confirmations[possible_page_offset] += 1
        best_confirmations = \
            max([(val, key) for (key, val) in page_offset_confirmations.items()])
        if best_confirmations[0] < 3: 
            continue
        next_best_confirmations = \
            max([2 * val for (key, val) in page_offset_confirmations.items() if key != best_confirmations[1]])
        if best_confirmations[0] > next_best_confirmations:
            return best_confirmations[1]
    
                    
    

#def find_text_items(reader, page_num, include_following_page = True, text_only = False):

def get_id(reader: PdfReader) -> str:
    if len(reader.pages) < 5:
        return None
    initial_pages_text = grab_text(reader, range(5)).lower()
    if "the mantis clan" in initial_pages_text:
        return "Mantis"
    if "courts of stone" in initial_pages_text:
        return "CoS"
    if "the land of ten thousand fortunes" in initial_pages_text:
        return "Core"
    if "welcome to the fringes of rokugan" in initial_pages_text:
        return "PoW"
    if "emerald empire" in initial_pages_text:
        return "EE"
    if "celestial realms" in initial_pages_text:
        return "CR"
    if "shadowlands" in initial_pages_text:
        return "SL"
    if "fields of victory" in initial_pages_text:
        return "FoV"
    if "game master's kit" in initial_pages_text:
        return "GMK"
    return None

def grab_text(reader, page_nums):
    text = ""
    for item in condense_text(find_text_items(reader, page_nums), []):
        text += item[0]
    return text
    
def chars_only(name: str):
    return ''.join([c for c in name.lower() if c.islower()])
    
def get_blurb(name: str, search_name: str, itemType: str, page_num: int, book: str, reader: PdfReader, 
              ignore_properties_list: list[str] = [], 
              beginning_properties: bool = False,
              cut_to_list: bool = False, verbose: bool = False) -> str:
    condensed_items = condense_text(find_text_items(reader, [page_num, page_num + 1]), beginning_properties)
    headings = \
        [(i, condensed_items[i][0]) for i in range(0, len(condensed_items)) 
                                    if 'Heading' == condensed_items[i][1]]
    possible_positions = \
        [i for (i, text) in headings if search_name.lower() in condensed_items[i][0].lower()]
    if len(possible_positions) == 0:
        possible_positions = \
            [i for (i, text) in headings if chars_only(search_name) in chars_only(text)]
    if len(possible_positions) == 0:
        matches = [(i, re.fullmatch(r"\s*(.*)\[(.*)\](.*)(\(.*\))?\s*", text)) for (i, text) in headings]
        possible_positions = [i for (i, m) in matches 
                                if m is not None and m.group(1).lower() in search_name.lower()]
        if len(possible_positions) == 0:
            possible_positions = [i for (i, m) in matches 
                                    if m is not None 
                                       and chars_only(m.group(1)) in chars_only(search_name)]
    if len(possible_positions) == 0:
        print(f"Could not find {itemType} {name} at {book} p. {page_num}.")
        return None
    heading_position = possible_positions[-1]
    
    blurb = ""
    i = heading_position + 1
    next_font = ''
    list_started = False
    while (i < len(condensed_items) and 'Heading' not in next_font
             and 'brushtip' not in next_font):
        next_font = condensed_items[i][1]
        if 'PropertyName' in next_font: 
            skip = False
            for prop_name in ignore_properties_list:
                if condensed_items[i][0].lower().startswith(prop_name):
                    skip = True
                    break
            if skip:
                i += 2
                continue
        
        if '$' in condensed_items[i][0]:
            blurb += condensed_items[i][0].replace('$', '\n-- ')
            if cut_to_list and not list_started:
                blurb = blurb[blurb.find('--'):]
            list_started = True
        elif cut_to_list and not list_started:
            i += 1
            continue
        elif 'New Opportunities' == condensed_items[i][0]:
            blurb += "\n\n*New Opportunities:*\n"
        elif 'Oblique' in next_font:
            i += 1
            continue
        elif itemType == "school" and "*ADVANCE" in condensed_items[i][0]:
            text = condensed_items[i][0] 
            blurb += text[:text.find("*ADVANCE")].strip()
            break
        elif 'Basic' == next_font:
            blurb += condensed_items[i][0].rstrip() + " "
        elif 'PropertyName' == next_font:
            if len(blurb) > 0 and not blurb.endswith('\n'):
                blurb += "\n"
            blurb += "*" + condensed_items[i][0].strip() + ":* "
        elif 'PropertyValue' == next_font:
            blurb += condensed_items[i][0] + "\n"
        if verbose:
            print(i, next_font, condensed_items[i][0])
        i = i + 1
    blurb = blurb.lstrip("\n")
    blurb = eliminate_extra_space(translate_icons(blurb.rstrip()))
    return {"name": name, "itemType": itemType, "text": blurb, 
            "book": book, "page": page_num}

def find_text_items(reader, page_nums, text_only = False):
    text_items = []
    if text_only: 
        visitor = lambda a,b,c,d,e: text_items.append(a)
    else:
        visitor = lambda a,b,c,d,e: text_items.append([a,b,c,d,e])
    for page_num in page_nums:
        reader.pages[page_num].extract_text(visitor_text=visitor)
    return text_items

def condense_text(text_items, beginning_properties) -> list[tuple[str, str]]:
    condensed_items = []
    current_font = ''
    aggregate_text = ''
    last_item = None
    icon_text = ""
    for i in range(0, len(text_items)):
        end_text = ''
        text_item = text_items[i]
        if text_item[3] is None:
            font = ''
        else:
            font = text_item[3]['/BaseFont']
        if text_item[0].startswith('\n') or text_items[i-1][0].endswith('\n'):
            new_line = True
        else:
            new_line = False
        if len(text_item[0].strip()) == 0:
            continue
        if text_item[2][0] < 8 and 'Ornament' not in font and last_item is not None \
           and last_item[3] is not None and 'Ornament' not in last_item[3]['/BaseFont']:
            continue
        if 'RPGIcons' in font:
            icon_text += text_item[0] + " "
        next_font = font
        next_text = ""
        if current_font != font:
            if 'BiolinumOB' in font:
                if 'Heading' != current_font:
                    next_font = 'Heading'
                    finish_item = True
                else:
                    finish_item = False
            elif text_item[0].rstrip().endswith(":") and 'Heavy' in font:
                next_font = 'PropertyName'
                if len(icon_text) > 0:
                    aggregate_text = aggregate_text[:-len(icon_text)]
                    next_text = icon_text
                    icon_text = ""
                while aggregate_text.strip().endswith("*"):
                    aggregate_text = aggregate_text.strip()[:-1]
                    heavy_init = aggregate_text.rfind('*')
                    next_text = aggregate_text[heavy_init+1:] + " " + next_text
                    aggregate_text = aggregate_text[:heavy_init].strip()
                finish_item = True
            elif current_font == 'PropertyName':
                next_font = 'PropertyValue'
                finish_item = True
            elif current_font == 'PropertyValue' and not new_line:
                finish_item = False
            elif current_font == 'PropertyValue' and not beginning_properties and 'Avenir' in font:
                finish_item = False
            elif 'Avenir' in font:
                if 'Basic' != current_font:
                    next_font = 'Basic'
                    finish_item = True
                else:
                    finish_item = False
            elif 'RPGIcons' in font:
                finish_item = False
            else:
                finish_item = True
            if finish_item:
                if len(aggregate_text.strip()) > 0:
                    condensed_items.append((remove_redundancy(aggregate_text.strip()), current_font))
                current_font = next_font
                aggregate_text = next_text
        if 'RPGIcons' not in font:
            icon_text = ""
        # Reproduce indentation
        if (len(aggregate_text) > 0 
             and text_item[2][4] - last_item[2][4] > 1
             and text_item[2][5] - last_item[2][5] < -1 
             and '$' not in aggregate_text):
            aggregate_text = aggregate_text.strip() + "\n\t"
        if current_font in ['Basic', 'PropertyValue']:
            if 'Oblique' in font:
                end_text = '/'
            if 'Heavy' in font:
                end_text += '*'
            aggregate_text += end_text[::-1]
            
        # Recombine words with weird font changes in the middle.
        if len(text_item[0]) == 1 and 'Avenir' in font:
            if aggregate_text.endswith(" "):
                aggregate_text = aggregate_text[:-1]
            if aggregate_text.endswith("/ /"):
                aggregate_text = aggregate_text[:-3]
            if aggregate_text.endswith("* *"):
                aggregate_text = aggregate_text[:-3]
        # Remove hyphenation
        if current_font == "PropertyName":
            aggregate_text += text_item[0].strip().rstrip(":")
        elif text_item[0][-1] == '-':
            aggregate_text += text_item[0][:-1].strip()
        elif text_item[0].endswith('-\n'):
            aggregate_text += text_item[0][:-2].strip()
        elif text_item[0][-1] == '\n':
            aggregate_text += text_item[0].strip()
            end_text += " "
        elif text_item[0][-1] == ' ':
            aggregate_text += text_item[0].strip()
            end_text += " "
        else:
            aggregate_text += text_item[0]
            end_text += " "
        aggregate_text += end_text
        last_item = text_item
    return condensed_items
    
def print_blurb(name, typeName, page, reader, **kwargs):
    print(typeName + " " + name)
    print(get_blurb(name, name, typeName, page, "", reader, **kwargs)["text"])
    print("\n\n")
    
def test_blurbs():
    reader = PdfReader('Legend_of_the_Five_Rings_Courts_of_Stone.pdf')
    print_blurb("Deer", "clan", 88, reader, beginning_properties = True)
    print_blurb("Shika", "family", 88, reader, beginning_properties = True, ignore_properties_list = ["ring increase", "skill increase", "glory"])
    print_blurb("Affect of Harmlessness", "distinction", 99, reader, cut_to_list = True)
    print_blurb("Well Connected", "distinction", 99, reader, cut_to_list = True)
    print_blurb("Famously Neutral", "distinction", 99, reader, cut_to_list = True)

    print_blurb("Local Flare", "passion", 100, reader, cut_to_list = True)
    print_blurb("Decorum", "passion", 100, reader, cut_to_list = True)
    print_blurb("Pot Stirrer", "passion", 101, reader, cut_to_list = True)
    
    print_blurb("Overconfidence", "adversity", 101, reader, cut_to_list = True)
    print_blurb("Lackluster", "adversity", 101, reader, cut_to_list = True)
    print_blurb("Unsavory Past", "adversity", 102, reader, cut_to_list = True)

    print_blurb("Isolation", "anxiety", 103, reader, cut_to_list = True)
    print_blurb("Web of Lies", "anxiety", 103, reader, cut_to_list = True)
    
    print_blurb("Ceremonial Tea Set", "gear", 105, reader)
    print_blurb("Folding Fan", "gear", 105, reader)
    print_blurb("Makeup Kit", "gear", 105, reader)    
    print_blurb("Mono Imi Fuda", "gear", 106, reader)
    print_blurb("Folding Half-Bow", "gear", 110, reader)

    print_blurb("Pole-Vault", "technique", 113, reader)
    print_blurb("Trip the Leg", "technique", 113, reader)
    print_blurb("Artful Alibi", "technique", 114, reader)
    print_blurb("Like a Ghost", "technique", 114, reader)
    print_blurb("Slicing Wind Kick", "technique", 115, reader)
    
    print_blurb("Bayushi Deathdealer", "school", 89, reader, 
                ignore_properties_list = SCHOOL_IGNORED_PROPERTIES)
    print_blurb("Shika Speardancer", "school", 96, reader, 
                ignore_properties_list = SCHOOL_IGNORED_PROPERTIES)
    print_blurb("Togashi Chronicler", "school", 97, reader, 
                ignore_properties_list = SCHOOL_IGNORED_PROPERTIES)

def scrape_all():
    pdfs = find_local_pdfs()
    get_clans(pdfs)
    get_schools(pdfs)
    get_items("distinction", pdfs, cut_to_list = True)
    print_blurb("Affect of Harmlessness", "distinction", 99, reader, cut_to_list = True)
    print_blurb("Local Flare", "passion", 100, reader, cut_to_list = True)
    print_blurb("Overconfidence", "adversity", 101, reader, cut_to_list = True)
    print_blurb("Isolation", "anxiety", 103, reader, cut_to_list = True)
    print_blurb("Ceremonial Tea Set", "gear", 105, reader)
    print_blurb("Pole-Vault", "technique", 113, reader)
    print_blurb("Bayushi Deathdealer", "school", 89, reader, 
                ignore_properties_list = SCHOOL_IGNORED_PROPERTIES)

def remove_redundancy(text) -> str:
    text = eliminate_extra_space(text)
    text = text.replace("/ /"," ")
    text = text.replace("* *"," ")
    text = text.replace("//","")
    text = text.replace("**","")
    return text

def eliminate_extra_space(text) -> str:
    size = len(text)
    text = text.replace("  "," ")
    text = text.replace(" \n", "\n")
    text = text.replace("\t\n", "\n")
    text = text.replace(" .",".")
    text = text.replace(" ,",",")
    text = text.replace("( ","(")
    text = text.replace(" )",")")
    text = text.replace("] [","][")
    text = text.replace("] :","]:")
    if text.startswith(" ") or text.startswith("\n"):
        text = text[1:]
    if len(text) < size:
        return eliminate_extra_space(text)
    else:
        return text
    
def translate_icons(text) -> str:
    text = text.replace('\uf3b0', "[Succ]")
    text = text.replace('\uf3b2', "[Exp]")
    text = text.replace('\uf3b5', "[RingDie]")
    text = text.replace('\uf3b9', "[Kata]")
    text = text.replace('\uf3ba', "[Shūji]")
    text = text.replace('\uf3b7', "[Ritual]")
    text = text.replace('\uf3bc', "[Invocation]")
    text = text.replace('\uf3b8', "[Ninjutsu]")
    text = text.replace('\uf3b8', "[Kihō]")
    return text


if __name__ == "__main__":
    make_user_description_file()