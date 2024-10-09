(* #load "str.cma";; *)
module StringMap = Map.Make (String)


let file = "./pride_prejudice.txt"
let stop_words_file = "./stop_words.txt"

let read_file (f: string): string = In_channel.with_open_text f In_channel.input_all;;

let split_text (s: string) (sw: string): string list = 
  let r = Str.regexp "[^a-zA-Z]" in
  let words = Str.split r s in 
  let content = read_file sw in
  let stop_words = Str.split r content in
  List.filter (fun x -> String.length x > 1 && not(List.exists (fun y -> y = x) stop_words)) words;;

let count_occ (acc: int StringMap.t) (w: string): int StringMap.t =
  let x = String.lowercase_ascii w in
  if StringMap.mem x acc then
    let old_value = StringMap.find x acc in
    StringMap.add x (old_value+1) acc
  else
    StringMap.add x 1 acc
  ;;

let frequency (f: string) (stop_words_file: string) (n: int): unit =
  let content = read_file f in
  let words = split_text content stop_words_file in
  let count_words = List.fold_left count_occ StringMap.empty words in
  let count_words_list = StringMap.bindings count_words in
  let sorted_count_words_list = List.sort (fun (_, v1) (_, v2) -> v2-v1) count_words_list in
  let n_words = List.fold_left 
    (fun (acc, count) x -> if count < n then (x :: acc, count + 1) else (acc, count)) 
    ([], 0) sorted_count_words_list in
  List.iter (fun k -> Printf.printf "(%s, %d)\n" (fst k) (snd k)) (List.rev (fst n_words));;

if Array.length Sys.argv != 3 then
  Printf.printf ("Il faut deux paramètres, le chemin du fichier txt et le nombre de mots\n")
else
  frequency Sys.argv.(1) stop_words_file (int_of_string Sys.argv.(2))