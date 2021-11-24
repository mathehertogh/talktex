# Generating your Own Simple Language Model 101
This quicky will show you how to generate a language model with a custom alphabet and phrase list. This language model can then be used in DeepSpeech in conjunction with any acoustic model.

## Requirements
1. A **DeepSpeech Native Client** package for your platform from [the GitHub releases](https://github.com/mozilla/DeepSpeech/releases/tag/v0.9.3)
2. The **generate_lm.py** script that can also be downloaded from [the DeepSpeech repo](https://github.com/mozilla/DeepSpeech/blob/master/data/lm/generate_lm.py)
3. **KenLM**, which can be downloaded from [the GitHub page](https://github.com/kpu/kenlm) and compiled using the included guide
4. An **alphabet.txt** file containing the characters that can be recognized by the acoustic model
5. A **phrases.txt** file containing the phrases you want to include in your language model

## Generating the scorer
1. Using the **phrases.txt** file, generate your language model and associated vocabulary using the **generate_lm.py** script:
		python3 generate_lm.py --input_txt phrases.txt --output_dir . \
		--top_k 1000 --kenlm_bins path/to/kenlm/build/bin/ \
		--arpa_order 5 --max_arpa_memory "85%" --arpa_prune "0|0|1" \
		--binary_a_bits 255 --binary_q_bits 8 --binary_type trie --discount_fallback
2. Use the resulting **vocab-1000.txt** and **lm.binary** files with your **alphabet.txt** to generate the scorer that can be used with DeepSpeech:
		./generate_scorer_package --alphabet alphabet.txt --lm lm.binary \
		--vocab vocab-1000.txt --package custom.scorer \
		--default_alpha 0.931289039105002 --default_beta 1.1834137581510284
3. Congrats! the resulting **custom.scorer** can be used with any DeepSpeech inference.
## Tuning
TBD

## More information
[DeepSpeech official documentation on creating language models](https://deepspeech.readthedocs.io/en/latest/Scorer.html)
